//
// Created by petr on 5/22/21.
//

#include "Range2D.h"
#include <pf_imgui/serialization.h>
#include <string>

namespace pf::ui::ig {

Range2D::Range2D(Range2D::Config &&config)
    : Range2D(config.name, config.label, config.min, config.max, config.value, config.size,
              config.persistent ? Persistent::Yes : Persistent::No) {}

Range2D::Range2D(std::string_view elementName, std::string_view labelText, glm::vec2 min, glm::vec2 max,
                 const math::Range<glm::vec2> &initialValue, Size s, Persistent persistent)
    : ItemElement(elementName), Savable(persistent), DragSource(false), DropTarget(false),
      label(std::string{labelText}), size(s), range(initialValue), minRange(min), maxRange(max) {}

void Range2D::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto &val = Prop_value(range);
  const auto oldVal = *range;
  if (ImWidgets::RangeSelect2D(label->get().c_str(), &val.start.x, &val.start.y, &val.end.x, &val.end.y, minRange.x,
                               minRange.y, maxRange.x, maxRange.y, static_cast<ImVec2>(*size))) {
    if (val != oldVal) { Prop_triggerListeners(range); }
  }
  drag(getValue());
  if (auto drop = dropAccept(); drop.has_value()) { *range.modify() = *drop; }
}

const glm::vec2 &Range2D::getMin() const { return minRange; }

void Range2D::setMin(const glm::vec2 &min) { minRange = min; }

const glm::vec2 &Range2D::getMax() const { return maxRange; }

void Range2D::setMax(const glm::vec2 &max) { maxRange = max; }

toml::table Range2D::toToml() const {
  const auto val = getValue();
  return toml::table{{"startValue", serializeGlmVec(val.start)}, {"endValue", serializeGlmVec(val.end)}};
}

void Range2D::setFromToml(const toml::table &src) {
  if (auto newValStartIter = src.find("startValue"); newValStartIter != src.end()) {
    if (auto newValStart = newValStartIter->second.as_array(); newValStart != nullptr) {
      if (newValStart->size() != 2) { return; }
      const auto start = safeDeserializeGlmVec<glm::vec2>(*newValStart);
      if (!start.has_value()) { return; }
      if (auto newValEndIter = src.find("endValue"); newValEndIter != src.end()) {
        if (auto newValEnd = newValEndIter->second.as_array(); newValEnd != nullptr) {
          if (newValEnd->size() != 2) { return; }
          const auto end = safeDeserializeGlmVec<glm::vec2>(*newValEnd);
          if (!end.has_value()) { return; }
          *range.modify() = {*start, *end};
        }
      }
    }
  }
}

const math::Range<glm::vec2> &Range2D::getValue() const { return *range; }

void Range2D::setValue(const math::Range<glm::vec2> &newValue) { *range.modify() = newValue; }

Subscription Range2D::addValueListenerImpl(std::function<void(const math::Range<glm::vec2> &)> listener) {
  return range.addListener(std::move(listener));
}

}  // namespace pf::ui::ig
