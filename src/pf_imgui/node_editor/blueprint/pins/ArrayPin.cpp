//
// Created by xflajs00 on 29.03.2022.
//

#include "ArrayPin.h"
#include "PinDraw.h"

namespace pf::ui::ig::bp {

ArrayPin::ArrayPin(const std::string &name, const std::string &label, const ImVec4 &color) : Pin(name, label, color) {}

void ArrayPin::renderIcon() {
  ImVec2 iconSize{16, 16};
  if (ImGui::IsRectVisible(iconSize)) {
    auto cursorPos = ImGui::GetCursorScreenPos();
    auto drawList = ImGui::GetWindowDrawList();

    drawPin3by3Quads(drawList, cursorPos + ImVec2{1, 3}, getColor(), hasAnyValidLinks());
  }
  ImGui::Dummy(iconSize);
}

}  // namespace pf::ui::ig::bp