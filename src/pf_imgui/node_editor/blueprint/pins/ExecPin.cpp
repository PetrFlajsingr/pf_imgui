//
// Created by xflajs00 on 29.03.2022.
//

#include "ExecPin.h"
#include "PinDraw.h"

namespace pf::ui::ig::bp {

ExecPin::ExecPin(const std::string &name, const std::string &label, const ImColor &pinColor)
    : Pin(name, label, pinColor) {}

void ExecPin::renderIcon() {
  ImVec2 iconSize{16, 16};
  if (ImGui::IsRectVisible(iconSize)) {
    auto cursorPos = ImGui::GetCursorScreenPos();
    auto drawList = ImGui::GetWindowDrawList();

    drawPinExec(drawList, cursorPos + ImVec2{3, 2}, getColor(), hasAnyValidLinks());
  }
  ImGui::Dummy(iconSize);
}

bool ExecPin::acceptsLinkWith(pf::ui::ig::Pin &other) const {
  if (!Pin::acceptsLinkWith(other)) { return false; }
  return dynamic_cast<ExecPin *>(&other) != nullptr;
}

}  // namespace pf::ui::ig::bp