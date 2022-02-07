//
// Created by petr on 10/31/20.
//

#include "Group.h"
#include <imgui.h>

namespace pf::ui::ig {

Group::Group(const std::string &elementName, std::unique_ptr<Resource<std::string>> label, Persistent persistent,
             AllowCollapse allowCollapse)
    : ItemElement(elementName), Labellable(std::move(label)), Collapsible(allowCollapse, persistent) {}

Group::Group(const std::string &elementName, std::unique_ptr<Resource<std::string>> label, AllowCollapse allowCollapse)
    : Group(elementName, std::move(label), Persistent::No, allowCollapse) {}

void Group::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto shouldBeOpen = !isCollapsed() || !isCollapsible();
  ImGui::SetNextItemOpen(shouldBeOpen);
  const auto flags = ImGuiTreeNodeFlags_DefaultOpen;
  setCollapsed(!ImGui::CollapsingHeader(getLabel().get().c_str(), flags));
  if (!isCollapsed()) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
  }
}

}  // namespace pf::ui::ig
