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
      Resizable(config.size), label(std::string{config.label.value}) {}

Button::Button(const std::string &name, const std::string &label, const Size &s, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), Resizable(s), label(label) {}

void Button::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::Button(label.get().c_str(), static_cast<ImVec2>(getSize()))) { notifyOnClick(); }
}

SmallButton::SmallButton(SmallButton::Config &&config)
    : ButtonBase(std::string{config.name.value}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      label(std::string{config.label.value}) {}

SmallButton::SmallButton(const std::string &name, const std::string &label, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), label(label) {}

void SmallButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::SmallButton(label.get().c_str())) { notifyOnClick(); }
}

ArrowButton::ArrowButton(ArrowButton::Config &&config)
    : ButtonBase(std::string{config.name.value}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      dir(config.direction) {}

ArrowButton::ArrowButton(const std::string &name, ArrowButton::Dir direction, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), dir(direction) {}

void ArrowButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::ArrowButton(getName().c_str(), static_cast<ImGuiDir>(dir))) { notifyOnClick(); }
}

ImageButton::ImageButton(ImageButton::Config &&config)
    : ButtonBase(std::string{config.name.value}, config.repeatable ? Repeatable::Yes : Repeatable::No),
      Resizable(config.size), texture(std::move(config.texture)) {}

ImageButton::ImageButton(const std::string &name, std::shared_ptr<Texture> tex, Size s, Repeatable isRepeatable)
    : ButtonBase(name, isRepeatable), Resizable(s), texture(std::move(tex)) {}

void ImageButton::setUVs(ImVec2 leftTop, ImVec2 rightBottom) {
  uvLeftTop = leftTop;
  uvRightBottom = rightBottom;
}

void ImageButton::setTexture(std::shared_ptr<Texture> tex) { texture = std::move(tex); }

void ImageButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::ImageButton(texture->getID(), static_cast<ImVec2>(getSize()), uvLeftTop, uvRightBottom)) {
    notifyOnClick();
  }
}

}  // namespace pf::ui::ig
