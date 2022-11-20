//
// Created by petr on 10/31/20.
//

#include "Button.h"
#include <imgui.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

ButtonBase::ButtonBase(std::string_view elementName, Repeatable isRepeatable)
    : ItemElement(elementName), repeatable(isRepeatable == Repeatable::Yes) {}

RAII ButtonBase::setButtonRepeat() {
  ImGui::PushButtonRepeat(repeatable);
  return RAII{ImGui::PopButtonRepeat};
}

InvisibleButton::InvisibleButton(InvisibleButton::Config &&config)
    : InvisibleButton(config.name, config.size, config.clickButton,
                      config.repeatable ? Repeatable::Yes : Repeatable::No) {}

InvisibleButton::InvisibleButton(std::string_view elementName, Size s, MouseButton clickButton, Repeatable isRepeatable)
    : ButtonBase(elementName, isRepeatable), size(s), clickBtn(clickButton) {}

void InvisibleButton::renderImpl() {
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::InvisibleButton(getName().c_str(), static_cast<ImVec2>(*size), static_cast<int>(clickBtn))) {
    Event_notify(clickEvent);
  }
}

Button::Button(Button::Config &&config)
    : Button(config.name, config.label, config.size, config.repeatable ? Repeatable::Yes : Repeatable::No) {}

Button::Button(std::string_view elementName, std::string_view labelText, Size s, Repeatable isRepeatable)
    : ButtonBase(elementName, isRepeatable), label(std::string{labelText}), size(s) {}

void Button::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::Button(label->get().c_str(), static_cast<ImVec2>(*size))) { Event_notify(clickEvent); }
}

SmallButton::SmallButton(SmallButton::Config &&config)
    : SmallButton(config.name, config.label, config.repeatable ? Repeatable::Yes : Repeatable::No) {}

SmallButton::SmallButton(std::string_view elementName, std::string_view labelText, Repeatable isRepeatable)
    : ButtonBase(elementName, isRepeatable), label(std::string{labelText}) {}

void SmallButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::SmallButton(label->get().c_str())) { Event_notify(clickEvent); }
}

ArrowButton::ArrowButton(ArrowButton::Config &&config)
    : ArrowButton(config.name, config.direction, config.repeatable ? Repeatable::Yes : Repeatable::No) {}

ArrowButton::ArrowButton(std::string_view elementName, ArrowButton::Dir direction, Repeatable isRepeatable)
    : ButtonBase(elementName, isRepeatable), dir(direction) {}

void ArrowButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::ArrowButton(getName().c_str(), static_cast<ImGuiDir>(dir))) { Event_notify(clickEvent); }
}

ImageButton::ImageButton(ImageButton::Config &&config)
    : ImageButton(config.name, config.texture, config.size, config.repeatable ? Repeatable::Yes : Repeatable::No) {}

ImageButton::ImageButton(std::string_view elementName, std::shared_ptr<Texture> tex, Size s, Repeatable isRepeatable)
    : ButtonBase(elementName, isRepeatable), size(s), texture(std::move(tex)) {}

void ImageButton::setUVs(ImVec2 leftTop, ImVec2 rightBottom) {
  uvLeftTop = leftTop;
  uvRightBottom = rightBottom;
}

void ImageButton::setTexture(std::shared_ptr<Texture> tex) { texture = std::move(tex); }

void ImageButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto repeat = setButtonRepeat();
  if (ImGui::ImageButton(getName().c_str(), texture->getID(), static_cast<ImVec2>(*size), uvLeftTop, uvRightBottom)) {
    Event_notify(clickEvent);
  }
}

}  // namespace pf::ui::ig
