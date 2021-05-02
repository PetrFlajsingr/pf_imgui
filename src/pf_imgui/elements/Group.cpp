//
// Created by petr on 10/31/20.
//

#include "Group.h"
#include <imgui.h>

namespace pf::ui::ig {

Group::Group(const std::string &elementName, const std::string &label, Persistent persistent,
             AllowCollapse allowCollapse)
    : ItemElement(elementName), Labellable(label), Collapsible(allowCollapse, persistent) {}

Group::Group(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse)
    : Group(elementName, label, Persistent::No, allowCollapse) {}

void Group::renderImpl() {
  const auto shouldBeOpen = !isCollapsed() || !isCollapsible();
  ImGui::SetNextItemOpen(shouldBeOpen);
  const auto flags = ImGuiTreeNodeFlags_DefaultOpen;
  setCollapsed(!ImGui::CollapsingHeader(getLabel().c_str(), flags));
  if (!isCollapsed()) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
  }
}

}// namespace pf::ui::ig