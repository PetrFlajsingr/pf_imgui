//
// Created by xflajs00 on 29.03.2022.
//

#include "EventPin.h"
#include "PinDraw.h"

namespace pf::ui::ig::bp {

EventPin::EventPin(const std::string &name, const std::string &label, const ImColor &color)
    : Pin(name, label, color) {}

void EventPin::renderIcon() {
  ImVec2 iconSize{16, 16};
  if (ImGui::IsRectVisible(iconSize)) {
    auto cursorPos = ImGui::GetCursorScreenPos();
    auto drawList = ImGui::GetWindowDrawList();

    const auto quadPos = cursorPos + ImVec2{3, 5};

    drawPinQuad(drawList, quadPos, getColor(), hasAnyValidLinks());
    drawPinArrow(drawList, quadPos + ImVec2{9, 3}, getColor());
  }
  ImGui::Dummy(iconSize);
}

}  // namespace pf::ui::ig::bp
