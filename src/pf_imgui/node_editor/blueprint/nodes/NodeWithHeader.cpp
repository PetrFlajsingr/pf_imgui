//
// Created by xflajs00 on 30.03.2022.
//

#include "NodeWithHeader.h"

namespace pf::ui::ig::bp {

NodeWithHeader::NodeWithHeader(const std::string &name, std::string label) : Node(name), label(std::move(label)) {}

void NodeWithHeader::renderHeader() {
  ImGui::BeginHorizontal((getName() + "_head").c_str());
  {
    ImGui::Spring(0);
    ImGui::Text(label.c_str());
    ImGui::Spring(1);
  }
  ImGui::EndHorizontal();
  const auto min = ImGui::GetItemRectMin();
  const auto max = ImGui::GetItemRectMax();
  ImGui::GetWindowDrawList()->AddLine(ImVec2{min.x, max.y}, max, ImGui::GetColorU32(ImGuiCol_Separator), 1.5f);
}

}  // namespace pf::ui::ig::bp