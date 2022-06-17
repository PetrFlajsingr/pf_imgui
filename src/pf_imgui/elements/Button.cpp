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
    : ButtonBase(std::string{config.name.value}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      Resizable(config.size), clickBtn(config.clickButton) {}

InvisibleButton::InvisibleButton(const std::string &elementName, const Size &s, MouseButton clickButton,
                                 Repeatable isRepeatable)
    : ButtonBase(elementName, isRepeatable), Resizable(s), clickBtn(clickButton) {}

void InvisibleButton::renderImpl() {
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::InvisibleButton(getName().c_str(), static_cast<ImVec2>(getSize()), static_cast<int>(clickBtn))) {
    notifyOnClick();
  }
}

Button::Button(Button::Config &&config)
    : ButtonBase(std::string{config.name.value}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      Labellable(std::string{config.label.value}), Resizable(config.size) {}

Button::Button(const std::string &name, const std::string &label, const Size &s, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), Labellable(label), Resizable(s) {}

void Button::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  [[maybe_unused]] auto scopedFont = applyFont();
  if (ImGui::Button(getLabel().c_str(), static_cast<ImVec2>(getSize()))) { notifyOnClick(); }
}

SmallButton::SmallButton(SmallButton::Config &&config)
    : ButtonBase(std::string{config.name.value}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      Labellable(std::string{config.label.value}) {}

SmallButton::SmallButton(const std::string &name, const std::string &label, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), Labellable(label) {}

void SmallButton::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  [[maybe_unused]] auto scopedFont = applyFont();
  if (ImGui::SmallButton(getLabel().c_str())) { notifyOnClick(); }
}

ArrowButton::ArrowButton(ArrowButton::Config &&config)
    : ButtonBase(std::string{config.name.value}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      dir(config.direction) {}

ArrowButton::ArrowButton(const std::string &name, ArrowButton::Dir direction, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), dir(direction) {}

void ArrowButton::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::ArrowButton(getName().c_str(), static_cast<ImGuiDir>(dir))) { notifyOnClick(); }
}

ImageButton::ImageButton(ImageButton::Config &&config)
    : ButtonBase(std::string{config.name.value}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      Resizable(config.size), textureId(config.textureId) {}

ImageButton::ImageButton(const std::string &name, ImTextureID texId, Size s, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), Resizable(s), textureId(texId) {}

void ImageButton::setUVs(ImVec2 leftTop, ImVec2 rightBottom) {
  uvLeftTop = leftTop;
  uvRightBottom = rightBottom;
}

void ImageButton::setTextureId(ImTextureID imTextureId) { textureId = imTextureId; }

void ImageButton::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::ImageButton(textureId, static_cast<ImVec2>(getSize()), uvLeftTop, uvRightBottom)) { notifyOnClick(); }
}

}  // namespace pf::ui::ig
