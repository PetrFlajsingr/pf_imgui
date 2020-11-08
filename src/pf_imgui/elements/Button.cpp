//
// Created by petr on 10/31/20.
//

#include "Button.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

Button::Button(const std::string &name, std::string caption, ButtonType buttonType,
               const ImVec2 &size)
    : Element(name), LabeledElement(name, std::move(caption)), ResizableElement(name, size),
      type(buttonType) {}

void Button::renderImpl() {
  switch (type) {
    case ButtonType::Normal:
      if (ImGui::Button(getLabel().c_str(), getSize())) { onClick(); }
      break;
    case ButtonType::Small:
      if (ImGui::SmallButton(getLabel().c_str())) { onClick(); }
      break;
    case ButtonType::ArrowUp:
      if (ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Up)) { onClick(); }
      break;
    case ButtonType::ArrowLeft:
      if (ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Left)) { onClick(); }
      break;
    case ButtonType::ArrowRight:
      if (ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Right)) { onClick(); }
      break;
    case ButtonType::ArrowDown:
      if (ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Down)) { onClick(); }
      break;
  }
}

ButtonType Button::getType() const { return type; }

void Button::setType(ButtonType buttonType) { type = buttonType; }
}// namespace pf::ui::ig