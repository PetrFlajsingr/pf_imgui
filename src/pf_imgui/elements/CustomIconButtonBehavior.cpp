//
// Created by xflajs00 on 17.04.2022.
//

#include "CustomIconButtonBehavior.h"
#include <imgui_internal.h>

namespace pf::ui::ig {
CustomIconButtonBehavior::CustomIconButtonBehavior(std::string_view elementName) : ItemElement(elementName) {}

Color CustomIconButtonBehavior::getIconColor() const { return iconColor; }

void CustomIconButtonBehavior::setIconColor(Color newColor) { iconColor = newColor; }

Size CustomIconButtonBehavior::getSize() const { return size; }

void CustomIconButtonBehavior::setSize(Size newSize) { size = newSize; }

void CustomIconButtonBehavior::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  auto drawList = ImGui::GetWindowDrawList();
  State state;
  if (*hovered || keepHighlighted) {
    if (*hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
      backgroundColor = Color{ImGui::GetColorU32(static_cast<ImGuiCol>(ColorOf::ButtonActive))};
      state = State::MouseDown;
    } else {
      backgroundColor = Color{ImGui::GetColorU32(static_cast<ImGuiCol>(ColorOf::ButtonHovered))};
      state = State::Hovered;
    }
  } else {
    backgroundColor = Color{ImGui::GetColorU32(static_cast<ImGuiCol>(ColorOf::Button))};
    state = State::None;
  }
  const auto cursorPos = ImGui::GetCursorScreenPos();
  drawList->AddRectFilled(cursorPos + ImVec2{0, 0}, cursorPos + static_cast<ImVec2>(size), static_cast<ImU32>(backgroundColor),
                          ImGui::GetStyle().FrameRounding);
  renderIcon(drawList, cursorPos);
  if (ImGui::InvisibleButton(getName().c_str(), static_cast<ImVec2>(size))) { state = State::Clicked; }
  update(state);
}

Color CustomIconButtonBehavior::getBackgroundColor() const { return backgroundColor; }

CustomIconButton::CustomIconButton(std::string_view elementName) : CustomIconButtonBehavior(elementName) {}

void CustomIconButton::update(CustomIconButtonBehavior::State state) {
  if (state == State::Clicked) { notifyClickEvent(); }
}

void CustomIconButton::notifyClickEvent() { Event_notify(clickEvent); }

CustomIconToggle::CustomIconToggle(std::string_view elementName, bool initValue, Persistent persistent)
    : CustomIconButtonBehavior(elementName), Savable(persistent), checked(initValue) {
  keepHighlighted = *checked;
}

toml::table CustomIconToggle::toToml() const { return toml::table{{"checked", getValue()}}; }

void CustomIconToggle::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("checked"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { *checked.modify() = *newVal; }
  }
}

void CustomIconToggle::update(CustomIconButtonBehavior::State state) {
  if (state == State::Clicked) { *checked.modify() = !*checked; }
  keepHighlighted = getValue();
}

const bool &CustomIconToggle::getValue() const { return *checked; }

void CustomIconToggle::setValue(const bool &newValue) { *checked.modify() = newValue; }

Subscription CustomIconToggle::addValueListenerImpl(std::function<void(const bool &)> listener) {
  return checked.addListener(std::move(listener));
}

}  // namespace pf::ui::ig