//
// Created by petr on 12/8/20.
//

#include "SliderAngle.h"

namespace pf::ui::ig {

SliderAngle::SliderAngle(const std::string &elementName, const std::string &caption, float min, float max, float value,
                         Persistent persistent, std::string format)
    : Element(elementName), ItemElement(elementName), LabeledElement(elementName, caption),
      ValueObservableElement(elementName, value), SavableElement(elementName, persistent), minDeg(min), maxDeg(max),
      format(std::move(format)) {}

void SliderAngle::renderImpl() {
  const auto oldValue = ValueObservableElement::getValue();
  ImGui::SliderAngle(getLabel().c_str(), getValueAddress(), minDeg, maxDeg, format.c_str());
  if (oldValue != ValueObservableElement::getValue()) { ValueObservableElement::notifyValueChanged(); }
}

void SliderAngle::unserialize_impl(const toml::table &src) {
  ValueObservableElement::setValueAndNotifyIfChanged(*src["value"].value<float>());
}

toml::table SliderAngle::serialize_impl() { return toml::table{{{"value", getValue()}}}; }

}// namespace pf::ui::ig