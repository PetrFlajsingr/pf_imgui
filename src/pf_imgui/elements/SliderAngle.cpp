//
// Created by petr on 12/8/20.
//

#include "SliderAngle.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

SliderAngle::SliderAngle(SliderAngle::Config &&config)
    : ItemElement(std::string{config.name.value}),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<float>(false), DropTarget<float>(false),
      label(std::string{config.label.value}), angle(config.value), minDeg(config.min), maxDeg(config.max),
      format(std::move(config.format)) {}

SliderAngle::SliderAngle(const std::string &elementName, const std::string &labelText, float min, float max,
                         float initialValue, Persistent persistent, std::string numberFormat)
    : ItemElement(elementName), Savable(persistent), DragSource<float>(false), DropTarget<float>(false),
      label(labelText), angle(initialValue), minDeg(min), maxDeg(max), format(std::move(numberFormat)) {}

void SliderAngle::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto flags = ImGuiSliderFlags_AlwaysClamp;
  if (ImGui::SliderAngle(label->get().c_str(), &angle.value, minDeg, maxDeg, format.c_str(), flags)) {
    angle.triggerListeners();
  }
  drag(getValue());
  if (auto drop = dropAccept(); drop.has_value()) {
    *angle.modify() = *drop;
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
    if (auto newVal = newValIter->second.value<float>(); newVal.has_value()) { *angle.modify() = *newVal; }
  }
}

const float &SliderAngle::getValue() const { return *angle; }

void SliderAngle::setValue(const float &newValue) { *angle.modify() = newValue; }

Subscription SliderAngle::addValueListenerImpl(std::function<void(const float &)> listener) {
  return angle.addListener(std::move(listener));
}

}  // namespace pf::ui::ig
