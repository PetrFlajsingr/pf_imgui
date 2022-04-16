//
// Created by xflajs00 on 30.03.2022.
//

#include "NodeWithLabel.h"
#include <pf_imgui/node_editor/blueprint/pins/Pin.h>

namespace pf::ui::ig::bp {

NodeWithLabel::NodeWithLabel(const std::string &name, std::string label) : Node(name), Labellable(std::move(label)) {}

toml::table NodeWithLabel::toToml() const {
  auto result = Node::toToml();
  result.insert_or_assign("label", getLabel());
  return result;
}

void NodeWithLabel::setFromToml(const toml::table &src) {
  Node::setFromToml(src);
  if (auto labelIter = src.find("label"); labelIter != src.end()) {
    if (auto labelToml = labelIter->second.as_string(); labelToml != nullptr) { setLabel(labelToml->get()); }
  }
}

std::unique_ptr<NodeWithLabel> NodeWithLabel::ConstructFromToml(ig::NodeEditor *parent, const toml::table &src) {
  auto result = std::make_unique<NodeWithLabel>("", "");
  result->parent = parent;
  result->setFromToml(src);
  return result;
}

void NodeWithLabel::renderHeader() {
  ImGui::BeginHorizontal("node_header");
  {
    ImGui::Spring(0);
    ImGui::Text(getLabel().c_str());
    ImGui::Spring(1);
  }
  ImGui::EndHorizontal();
  ImGui::Spring(0.f);

  const auto min = ImGui::GetItemRectMin();
  const auto max = ImGui::GetItemRectMax();
  const auto nodeSize = ax::NodeEditor::GetNodeSize(getId());
  const auto paddingOffset = 2 * ax::NodeEditor::GetStyle().NodePadding.x;
  ImGui::GetWindowDrawList()->AddLine(ImVec2{min.x, max.y}, ImVec2{min.x + nodeSize.x - paddingOffset, max.y},
                                      ImGui::GetColorU32(ImGuiCol_Separator), 1.5f);
}

}  // namespace pf::ui::ig::bp