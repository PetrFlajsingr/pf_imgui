//
// Created by petr on 10/31/20.
//

#include "Tree.h"
#include <imgui.h>

namespace pf::ui::ig {

Tree::Tree(const std::string &elementName, const std::string &label, Persistent persistent)
    : Tree(elementName, label, AllowCollapse::Yes, persistent) {}

Tree::Tree(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse, Persistent persistent)
    : Element(elementName), Labellable(label), Collapsible(allowCollapse, persistent) {
  setCollapsed(true);
}

Tree &Tree::addNode(const std::string &elementName, const std::string &caption, AllowCollapse allowCollapse) {
  return createChild<Tree>(elementName, caption, allowCollapse, isPersistent() ? Persistent::Yes : Persistent::No);
}
void Tree::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto shouldBeOpen = !isCollapsed() || !isCollapsible();
  ImGui::SetNextItemOpen(shouldBeOpen);
  setCollapsed(!ImGui::TreeNode(getLabel().c_str()));
  if (!isCollapsed()) {
    RAII end{[] { ImGui::TreePop(); }};
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
  }
}

}// namespace pf::ui::ig