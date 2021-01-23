//
// Created by petr on 10/31/20.
//

#include "Group.h"
#include <imgui.h>

namespace pf::ui::ig {

Group::Group(const std::string &elementName, const std::string &caption, bool allowCollapse)
    : Element(elementName), Container(elementName), LabeledElement(elementName, caption), collapsible(allowCollapse) {}

void Group::renderImpl() {
  ImGui::BeginGroup();
  ImGui::Text("%s:", getLabel().c_str());
  if (collapsible) {
    ImGui::SameLine();
    if (ImGui::Button(isCollapsed() ? "Show" : "Collapse")) { setCollapsed(!isCollapsed()); }
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