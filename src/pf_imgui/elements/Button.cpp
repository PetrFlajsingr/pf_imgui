//
// Created by petr on 10/31/20.
//

#include "Button.h"
#include <imgui.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

Button::Button(const std::string &name, std::string label, ButtonType buttonType, bool isRepeatable, const ImVec2 &size)
    : ItemElement(name), Labellable(std::move(label)), Resizable(size), type(buttonType), repeatable(isRepeatable) {}

void Button::renderImpl() {
  ImGui::PushButtonRepeat(repeatable);
  auto disableRepeat = RAII{[] { ImGui::PopButtonRepeat(); }};
  auto wasClicked = false;
  switch (type) {
    case ButtonType::Normal: wasClicked = ImGui::Button(getLabel().c_str(), getSize()); break;
    case ButtonType::Small: wasClicked = ImGui::SmallButton(getLabel().c_str()); break;
    case ButtonType::ArrowUp: wasClicked = ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Up); break;
    case ButtonType::ArrowLeft: wasClicked = ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Left); break;
    case ButtonType::ArrowRight: wasClicked = ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Right); break;
    case ButtonType::ArrowDown: wasClicked = ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Down); break;
    case ButtonType::Invisible: wasClicked = ImGui::InvisibleButton(getLabel().c_str(), getSize()); break;
  }
  if (wasClicked) { notifyOnClick(); }
}

ButtonType Button::getType() const { return type; }

void Button::setType(ButtonType buttonType) { type = buttonType; }
bool Button::isRepeatable() const { return repeatable; }
void Button::setRepeatable(bool repeatable) { Button::repeatable = repeatable; }

}// namespace pf::ui::ig