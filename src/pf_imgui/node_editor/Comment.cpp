//
// Created by xflajs00 on 02.04.2022.
//

#include "Comment.h"
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Comment::Comment(const std::string &name, const std::string &label, Size initSize)
    : NodeBase(name), Labellable(label), Resizable(initSize) {}

Comment::Comment(const std::string &name, const Position &initPosition, const std::string &label, const Size &s)
    : NodeBase(name, initPosition), Labellable(label), Resizable(s) {}

void Comment::setSize(const Size &s) {
  Resizable::setSize(s);
  sizeDirty = true;
}

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
      if (sizeDirty) {
        sizeDirty = false;
        ax::NodeEditor::SetGroupSize(getId(), static_cast<ImVec2>(getSize()));
      }
      ImGui::BeginHorizontal("header");
      {
        ImGui::Spring(1);
        ImGui::Text(getLabel().c_str());
        ImGui::Spring(1);
      }
      ImGui::EndHorizontal();
      ax::NodeEditor::Group(static_cast<ImVec2>(getSize()));
      const auto commentSize = ax::NodeEditor::GetNodeSize(getId());
      setSizeInner(Size{commentSize});
    }
    ImGui::EndVertical();
  }
}
}  // namespace pf::ui::ig