//
// Created by xflajs00 on 21.05.2022.
//

#include "IndeterminateProgressBar.h"
#include <pf_imgui/details/IndeterminateProgressBar.h>

namespace pf::ui::ig {

IndeterminateProgressBar::IndeterminateProgressBar(IndeterminateProgressBar::Config &&config)
    : IndeterminateProgressBar(std::string{config.name.value}, config.overlay, config.speed, config.size) {}

IndeterminateProgressBar::IndeterminateProgressBar(const std::string &elementName, std::string overlayStr, float speed,
                                                   const Size &initialSize)
    : ItemElement(elementName), Resizable(initialSize), changeSpeed(speed), overlay(std::move(overlayStr)) {}

void IndeterminateProgressBar::setSpeed(float speed) { changeSpeed = speed; }

float IndeterminateProgressBar::getSpeed() const { return changeSpeed; }

void IndeterminateProgressBar::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGui::IndeterminateProgressBar(static_cast<float>(ImGui::GetTime()) * -changeSpeed, static_cast<ImVec2>(getSize()),
                                  overlay.c_str());
}

}  // namespace pf::ui::ig