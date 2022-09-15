//
// Created by xflajs00 on 29.03.2022.
//

#include "EventPin.h"
#include "PinDraw.h"

namespace pf::ui::ig::bp {

EventPin::EventPin(std::string_view elementName, std::string_view labelText, Color pinColor)
    : Pin(elementName, labelText, pinColor) {}

bool EventPin::acceptsLinkWith(ig::Pin &other) const {
  if (!Pin::acceptsLinkWith(other)) { return false; }
  return dynamic_cast<EventPin *>(&other) != nullptr;
}

std::unique_ptr<EventPin> EventPin::ConstructFromToml(ig::Node *parent, const toml::table &src) {
  auto result = std::make_unique<EventPin>("", "", Color::White);
  result->parent = parent;
  result->setFromToml(src);
  return result;
}

void EventPin::renderIcon() {
  ImVec2 iconSize{16, 16};
  if (ImGui::IsRectVisible(iconSize)) {
    auto cursorPos = ImGui::GetCursorScreenPos();
    auto drawList = ImGui::GetWindowDrawList();

    const auto quadPos = cursorPos + ImVec2{3, 5};

    drawPinQuad(drawList, quadPos, static_cast<ImVec4>(getColor()), hasAnyValidLinks());
    drawPinArrow(drawList, quadPos + ImVec2{9, 3}, static_cast<ImVec4>(getColor()));
  }
  ImGui::Dummy(iconSize);
}

}  // namespace pf::ui::ig::bp
