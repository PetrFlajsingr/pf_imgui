//
// Created by petr on 12/8/20.
//

#include "SliderAngle.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

SliderAngle::SliderAngle(SliderAngle::Config &&config)
    : ItemElement(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<float>(false), DropTarget<float>(false),
      minDeg(config.min), maxDeg(config.max), format(std::move(config.format)) {}

SliderAngle::SliderAngle(const std::string &elementName, const std::string &label, float min, float max, float value,
                         Persistent persistent, std::string format)
    : ItemElement(elementName), Labellable(label), ValueObservable(value),
      Savable(persistent), DragSource<float>(false), DropTarget<float>(false), minDeg(min), maxDeg(max),
      format(std::move(format)) {}

void SliderAngle::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto flags = ImGuiSliderFlags_AlwaysClamp;
  if (ImGui::SliderAngle(getLabel().c_str(), getValueAddress(), minDeg, maxDeg, format.c_str(), flags)) {
    ValueObservable::notifyValueChanged();
  }
  drag(getValue());
  if (auto drop = dropAccept(); drop.has_value()) {
    setValueAndNotifyIfChanged(*drop);
    return;
  }
}

float SliderAngle::getMinDeg() const { return minDeg; }

void SliderAngle::setMinDeg(float min) { SliderAngle::minDeg = min; }

float SliderAngle::getMaxDeg() const { return maxDeg; }

void SliderAngle::setMaxDeg(float max) { SliderAngle::maxDeg = max; }

toml::table SliderAngle::toToml() const { return toml::table{{"value", getValue()}}; }

void SliderAngle::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<float>(); newVal.has_value()) {
      ValueObservable<float>::setValueAndNotifyIfChanged(*newVal);
    }
  }
}

}  // namespace pf::ui::ig
