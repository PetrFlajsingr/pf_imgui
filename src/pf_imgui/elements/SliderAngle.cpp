//
// Created by petr on 12/8/20.
//

#include "SliderAngle.h"

namespace pf::ui::ig {

SliderAngle::SliderAngle(const std::string &elementName, const std::string &label, float min, float max, float value,
                         Persistent persistent, std::string format)
    : ItemElement(elementName), Labellable(label), ValueObservable(value), Savable(persistent), minDeg(min),
      maxDeg(max), format(std::move(format)) {}

void SliderAngle::renderImpl() {
  const auto oldValue = ValueObservable::getValue();
  ImGui::SliderAngle(getLabel().c_str(), getValueAddress(), minDeg, maxDeg, format.c_str());
  if (oldValue != ValueObservable::getValue()) { ValueObservable::notifyValueChanged(); }
}

void SliderAngle::unserialize_impl(const toml::table &src) {
  ValueObservable::setValueAndNotifyIfChanged(*src["value"].value<float>());
}

toml::table SliderAngle::serialize_impl() { return toml::table{{{"value", getValue()}}}; }

}// namespace pf::ui::ig