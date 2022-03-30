//
// Created by xflajs00 on 29.03.2022.
//

#include "ValuePin.h"

namespace pf::ui::ig::bp {

ValuePin::ValuePin(const std::string &name, const std::string &label, const ImColor &color) : Pin(name, label, color) {}

void ValuePin::renderIcon() {
  const static auto renderAreaSize = ImVec2{16, 16};
  if (ImGui::IsRectVisible(renderAreaSize)) {
    auto cursorPos = ImGui::GetCursorScreenPos();
    auto drawList = ImGui::GetWindowDrawList();

    const auto circleCenter = cursorPos + ImVec2{6, 8};

    drawPinCircle(drawList, circleCenter, getColor(), hasAnyValidLinks());
    drawPinArrow(drawList, circleCenter + ImVec2{6, 0}, getColor());
  }
  ImGui::Dummy(renderAreaSize);
}

}  // namespace pf::ui::ig::bp