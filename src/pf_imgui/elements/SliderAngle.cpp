//
// Created by petr on 12/8/20.
//

#include "SliderAngle.h"
#include <imgui.h>

namespace pf::ui::ig {

SliderAngle::SliderAngle(const std::string &elementName, const std::string &label, float min, float max, float value,
                         Persistent persistent, std::string format)
    : ItemElement(elementName), Labellable(label), ValueObservable(value), Savable(persistent), minDeg(min),
      maxDeg(max), format(std::move(format)) {}

void SliderAngle::renderImpl() {
  if (ImGui::SliderAngle(getLabel().c_str(), getValueAddress(), minDeg, maxDeg, format.c_str())) {
    ValueObservable::notifyValueChanged();
  }
}

void SliderAngle::unserialize_impl(const toml::table &src) {
  ValueObservable::setValueAndNotifyIfChanged(*src["value"].value<float>());
}

toml::table SliderAngle::serialize_impl() { return toml::table{{{"value", getValue()}}}; }
float SliderAngle::getMinDeg() const { return minDeg; }
void SliderAngle::setMinDeg(float minDeg) { SliderAngle::minDeg = minDeg; }
float SliderAngle::getMaxDeg() const { return maxDeg; }
void SliderAngle::setMaxDeg(float maxDeg) { SliderAngle::maxDeg = maxDeg; }

}// namespace pf::ui::ig