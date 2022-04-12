//
// Created by petr on 10/31/20.
//

#include "Button.h"
#include <imgui.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

ButtonBase::ButtonBase(const std::string &name, Repeatable isRepeatable)
    : ItemElement(name), repeatable(isRepeatable == Repeatable::Yes) {}

bool ButtonBase::isRepeatable() const { return repeatable; }

void ButtonBase::setRepeatable(bool newRepeatable) { repeatable = newRepeatable; }

RAII ButtonBase::setButtonRepeat() {
  ImGui::PushButtonRepeat(isRepeatable());
  return RAII{ImGui::PopButtonRepeat};
}

InvisibleButton::InvisibleButton(InvisibleButton::Config &&config)
    : ButtonBase(std::string{config.name}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      Resizable(config.size), clickBtn(config.clickButton) {}

InvisibleButton::InvisibleButton(const std::string &elementName, const Size &s, MouseButton clickButton,
                                 Repeatable isRepeatable)
    : ButtonBase(elementName, isRepeatable), Resizable(s), clickBtn(clickButton) {}

void InvisibleButton::renderImpl() {
  auto repeat = setButtonRepeat();
  if (ImGui::InvisibleButton(getName().c_str(), static_cast<ImVec2>(getSize()), static_cast<int>(clickBtn))) {
    notifyOnClick();
  }
}

Button::Button(Button::Config &&config)
    : ButtonBase(std::string{config.name}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      Labellable(std::string{config.label}), Resizable(config.size) {}

Button::Button(const std::string &name, const std::string &label, const Size &s, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), Labellable(label), Resizable(s) {}

void Button::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  auto repeat = setButtonRepeat();
  if (ImGui::Button(getLabel().c_str(), static_cast<ImVec2>(getSize()))) { notifyOnClick(); }
}

SmallButton::SmallButton(SmallButton::Config &&config)
    : ButtonBase(std::string{config.name}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      Labellable(std::string{config.label}) {}

SmallButton::SmallButton(const std::string &name, const std::string &label, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), Labellable(label) {}

void SmallButton::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  auto repeat = setButtonRepeat();
  if (ImGui::SmallButton(getLabel().c_str())) { notifyOnClick(); }
}

ArrowButton::ArrowButton(ArrowButton::Config &&config)
    : ButtonBase(std::string{config.name}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      dir(config.direction) {}

ArrowButton::ArrowButton(const std::string &name, ArrowButton::Dir direction, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), dir(direction) {}

void ArrowButton::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  auto repeat = setButtonRepeat();
  if (ImGui::ArrowButton(getName().c_str(), static_cast<ImGuiDir>(dir))) { notifyOnClick(); }
}

}  // namespace pf::ui::ig
