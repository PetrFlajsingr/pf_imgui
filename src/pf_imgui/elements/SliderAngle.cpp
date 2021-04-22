//
// Created by petr on 12/8/20.
//

#include "SliderAngle.h"
#include <imgui.h>

namespace pf::ui::ig {

SliderAngle::SliderAngle(const std::string &elementName, const std::string &label, float min, float max, float value,
                         Persistent persistent, std::string format)
    : ItemElement(elementName), Labellable(label), ValueObservable(value),
      Savable(persistent), DragSource<float>(false), DropTarget<float>(false), minDeg(min), maxDeg(max),
      format(std::move(format)) {}

void SliderAngle::renderImpl() {
  if (ImGui::SliderAngle(getLabel().c_str(), getValueAddress(), minDeg, maxDeg, format.c_str())) {
    ValueObservable::notifyValueChanged();
  }
  drag(getValue());
  if (auto drop = dropAccept(); drop.has_value()) {
    setValueAndNotifyIfChanged(*drop);
    return;
  }
}

void SliderAngle::unserialize_impl(const toml::table &src) {
  ValueObservable::setValueAndNotifyIfChanged(*src["value"].value<float>());
}

toml::table SliderAngle::serialize_impl() { return toml::table{{{"value", getValue()}}}; }
float SliderAngle::getMinDeg() const { return minDeg; }
void SliderAngle::setMinDeg(float min) { SliderAngle::minDeg = min; }
float SliderAngle::getMaxDeg() const { return maxDeg; }
void SliderAngle::setMaxDeg(float max) { SliderAngle::maxDeg = max; }

}// namespace pf::ui::ig