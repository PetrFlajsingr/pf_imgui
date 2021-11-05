//
// Created by petr on 10/31/20.
//

#include "Button.h"
#include <imgui.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

Button::Button(const std::string &name, std::string label, ButtonType buttonType, Repeatable isRepeatable,
               const Size &size)
    : ItemElement(name), Labellable(std::move(label)), Resizable(size), type(buttonType),
      repeatable(isRepeatable == Repeatable::Yes) {}

void Button::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  ImGui::PushButtonRepeat(repeatable);
  auto disableRepeat = RAII{[] { ImGui::PopButtonRepeat(); }};
  auto wasClicked = false;
  switch (type) {
    case ButtonType::Normal: wasClicked = ImGui::Button(getLabel().c_str(), getSize().asImVec()); break;
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

bool Button::isRepeatable() const { return repeatable; }

void Button::setRepeatable(bool newRepeatable) { repeatable = newRepeatable; }

InvisibleButton::InvisibleButton(const std::string &elementName, const Size &s, MouseButton clickButton,
                                 Repeatable isRepeatable)
    : ItemElement(elementName), Resizable(s), repeatable(isRepeatable == Repeatable::Yes), clickBtn(clickButton) {}

bool InvisibleButton::isRepeatable() const { return false; }

void InvisibleButton::setRepeatable(bool repeatable) {}

void InvisibleButton::renderImpl() {
  ImGui::PushButtonRepeat(repeatable);
  auto disableRepeat = RAII{[] { ImGui::PopButtonRepeat(); }};
  if (ImGui::InvisibleButton(getName().c_str(), getSize().asImVec(), static_cast<int>(clickBtn))) { notifyOnClick(); }
}

}// namespace pf::ui::ig