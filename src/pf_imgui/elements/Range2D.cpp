//
// Created by petr on 5/22/21.
//

#include "Range2D.h"
#include <pf_imgui/serialization.h>

namespace pf::ui::ig {

Range2D::Range2D(const std::string &elementName, const std::string &label, const glm::vec2 &min, const glm::vec2 &max,
                 const math::Range<glm::vec2> &value, const Size &s, Persistent persistent)
    : ItemElement(elementName), Labellable(label), ValueObservable(value), Resizable(s), Savable(persistent),
      DragSource(false), DropTarget(false), minRange(min), maxRange(max) {}

void Range2D::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  auto val = getValueAddress();
  const auto oldVal = getValue();
  if (ImWidgets::RangeSelect2D(getLabel().c_str(), &val->start.x, &val->start.y, &val->end.x, &val->end.y, minRange.x,
                               minRange.y, maxRange.x, maxRange.y, getSize().asImVec())) {
    if (*val != oldVal) { notifyValueChanged(); }
  }
  drag(getValue());
  if (auto drop = dropAccept(); drop.has_value()) { setValueAndNotifyIfChanged(*drop); }
}

const glm::vec2 &Range2D::getMin() const { return minRange; }

void Range2D::setMin(const glm::vec2 &min) { minRange = min; }

const glm::vec2 &Range2D::getMax() const { return maxRange; }

void Range2D::setMax(const glm::vec2 &max) { maxRange = max; }

void Range2D::unserialize_impl(const toml::table &src) {
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

toml::table Range2D::serialize_impl() const {
  const auto val = getValue();
  return toml::table{{{"startValue", serializeGlmVec(val.start)}, {"endValue", serializeGlmVec(val.end)}}};
}

}// namespace pf::ui::ig