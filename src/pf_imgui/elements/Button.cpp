//
// Created by petr on 10/31/20.
//

#include "Button.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

Button::Button(const std::string &name, std::string caption, ButtonType buttonType, const ImVec2 &size)
    : Element(name), LabeledElement(name, std::move(caption)), ResizableElement(name, size), type(buttonType) {}

void Button::renderImpl() {
  auto wasClicked = false;
  switch (type) {
    case ButtonType::Normal: wasClicked = ImGui::Button(getLabel().c_str(), getSize()); break;
    case ButtonType::Small: wasClicked = ImGui::SmallButton(getLabel().c_str()); break;
    case ButtonType::ArrowUp: wasClicked = ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Up); break;
    case ButtonType::ArrowLeft: wasClicked = ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Left); break;
    case ButtonType::ArrowRight: wasClicked = ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Right); break;
    case ButtonType::ArrowDown: wasClicked = ImGui::ArrowButton(getLabel().c_str(), ImGuiDir_::ImGuiDir_Down); break;
  }
  if (wasClicked) { notifyOnClick(); }
}

ButtonType Button::getType() const { return type; }

void Button::setType(ButtonType buttonType) { type = buttonType; }

}// namespace pf::ui::ig