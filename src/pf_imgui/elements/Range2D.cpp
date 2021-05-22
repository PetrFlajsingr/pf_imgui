//
// Created by petr on 5/22/21.
//

#include "Range2D.h"
#include <pf_imgui/serialization.h>

namespace pf::ui::ig {

Range2D::Range2D(const std::string &elementName, const std::string &label, const glm::vec2 &min,
                 const glm::vec2 &max, const math::Range<glm::vec2> &value, const Size &s, Persistent persistent)
    : ItemElement(elementName), Labellable(label), ValueObservable(value), Resizable(s), Savable(persistent),
      DragSource(false), DropTarget(false), minRange(min), maxRange(max) {}

void Range2D::renderImpl() {
  auto val = getValueAddress();
  const auto oldVal = getValue();
  if (ImWidgets::RangeSelect2D(getLabel().c_str(), &val->start.x, &val->start.y, &val->end.x, &val->start.y, minRange.x,
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
  const auto tomlStartVec = src["value"]["startValue"].as_array();
  const auto tomlEndVec = src["value"]["endValue"].as_array();
  const auto start = deserializeGlmVec<glm::vec2>(*tomlStartVec);
  const auto end = deserializeGlmVec<glm::vec2>(*tomlEndVec);
  setValueAndNotifyIfChanged({start, end});
}

toml::table Range2D::serialize_impl() {
  const auto value = getValue();
  return toml::table{{{"startValue", serializeGlmVec(value.start)}, {"endValue", serializeGlmVec(value.end)}}};
}

}// namespace pf::ui::ig