//
// Created by xflajs00 on 30.03.2022.
//

#include "SimpleNode.h"

namespace pf::ui::ig::bp {

SimpleNode::SimpleNode(std::string_view elementName) : Node(elementName) {}

void SimpleNode::renderMiddle() {
  ImGui::BeginVertical((getName() + "_mid").c_str());
  {
    ImGui::Spring(0.5);
    renderIcon();
    ImGui::Spring(0.5);
  }
  ImGui::EndVertical();
}

}  // namespace pf::ui::ig::bp