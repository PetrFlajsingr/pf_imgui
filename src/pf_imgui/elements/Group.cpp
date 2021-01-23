//
// Created by petr on 10/31/20.
//

#include "Group.h"
#include <imgui.h>

namespace pf::ui::ig {

Group::Group(const std::string &elementName, const std::string &caption, AllowCollapse allowCollapse)
    : Element(elementName), Container(elementName), LabeledElement(elementName, caption), collapsible(allowCollapse) {}

void Group::renderImpl() {
  ImGui::BeginGroup();
  ImGui::Text("%s:", getLabel().c_str());
  if (collapsible == AllowCollapse::Yes) {
    ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetItemRectSize().x);
    if (ImGui::ArrowButton((getName() + "collapse_btn").c_str(),
                           isCollapsed() ? ImGuiDir_::ImGuiDir_Up : ImGuiDir_::ImGuiDir_Down)) {
      setCollapsed(!isCollapsed());
    }
  }
  ImGui::Separator();
  if (!isCollapsed()) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
  }
  ImGui::Separator();
  ImGui::EndGroup();
}

void Group::collapse_impl(bool) {}

}// namespace pf::ui::ig