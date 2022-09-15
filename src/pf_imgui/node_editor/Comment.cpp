//
// Created by xflajs00 on 02.04.2022.
//

#include "Comment.h"
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Comment::Comment(std::string_view elementName, std::string_view labelText, Size initialSize)
    : NodeBase(elementName), label(std::string{labelText}), size(initialSize) {
  size.addListener([this](auto) { sizeDirty = true; });
}

Comment::Comment(std::string_view elementName, Position initPosition, std::string_view labelText, Size s)
    : NodeBase(elementName, initPosition), label(std::string{labelText}), size(s) {
  size.addListener([this](auto) { sizeDirty = true; });
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

    [[maybe_unused]] auto stylePop = RAII{[] {
      ImGui::PopStyleVar();
      ax::NodeEditor::PopStyleColor(2);
    }};

    ax::NodeEditor::BeginNode(getId());
    [[maybe_unused]] auto endNode = RAII{ax::NodeEditor::EndNode};
    ImGui::BeginVertical("content");
    {
      if (sizeDirty) {
        sizeDirty = false;
        ax::NodeEditor::SetGroupSize(getId(), static_cast<ImVec2>(*size));
      }
      ImGui::BeginHorizontal("header");
      {
        ImGui::Spring(1);
        ImGui::Text(label->get().c_str());
        ImGui::Spring(1);
      }
      ImGui::EndHorizontal();
      ax::NodeEditor::Group(static_cast<ImVec2>(*size));
      const auto commentSize = ax::NodeEditor::GetNodeSize(getId());
      *size.modify() = Size{commentSize};
      sizeDirty = false;
    }
    ImGui::EndVertical();
  }
}
}  // namespace pf::ui::ig