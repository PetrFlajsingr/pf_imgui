//
// Created by petr on 5/22/21.
//

#include "Range2D.h"
#include <pf_imgui/serialization.h>
#include <string>

namespace pf::ui::ig {

Range2D::Range2D(Range2D::Config &&config)
    : ItemElement(std::string{config.name.value}), ValueObservable(config.value), Resizable(config.size),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource(false), DropTarget(false),
      label(std::string{config.label.value}), minRange(config.min), maxRange(config.max) {}

Range2D::Range2D(const std::string &elementName, const std::string &labelText, const glm::vec2 &min,
                 const glm::vec2 &max, const math::Range<glm::vec2> &initialValue, const Size &s, Persistent persistent)
    : ItemElement(elementName), ValueObservable(initialValue), Resizable(s), Savable(persistent), DragSource(false),
      DropTarget(false), label(labelText), minRange(min), maxRange(max) {}

void Range2D::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto val = getValueAddress();
  const auto oldVal = getValue();
  if (ImWidgets::RangeSelect2D(label.get().c_str(), &val->start.x, &val->start.y, &val->end.x, &val->end.y, minRange.x,
                               minRange.y, maxRange.x, maxRange.y, static_cast<ImVec2>(getSize()))) {
    if (*val != oldVal) { notifyValueChanged(); }
  }
  drag(getValue());
  if (auto drop = dropAccept(); drop.has_value()) { setValueAndNotifyIfChanged(*drop); }
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
          setValueAndNotifyIfChanged({*start, *end});
        }
      }
    }
  }
}

}  // namespace pf::ui::ig
