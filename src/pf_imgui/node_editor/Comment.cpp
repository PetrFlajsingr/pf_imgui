//
// Created by xflajs00 on 02.04.2022.
//

#include "Comment.h"
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Comment::Comment(const std::string &name, const std::string &label, Size initSize)
    : NodeBase(name), Labellable(label), size(static_cast<ImVec2>(initSize)) {}

void Comment::renderImpl() {
  {
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.75);

    auto bgColor = ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBg];
    bgColor.w = 0.25;
    auto borderColor = ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBorder];
    borderColor.w = 0.25;
    ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBg, bgColor);
    ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, borderColor);

    auto stylPop = RAII{[] {
      ImGui::PopStyleVar();
      ax::NodeEditor::PopStyleColor(2);
    }};

    ax::NodeEditor::BeginNode(getId());
    auto endNode = RAII{ax::NodeEditor::EndNode};
    ImGui::BeginVertical("content");
    {
      ImGui::BeginHorizontal("header");
      {
        ImGui::Spring(1);
        ImGui::Text(getLabel().c_str());
        ImGui::Spring(1);
      }
      ImGui::EndHorizontal();
      ax::NodeEditor::Group(size);
    }
    ImGui::EndVertical();
  }
}

}  // namespace pf::ui::ig