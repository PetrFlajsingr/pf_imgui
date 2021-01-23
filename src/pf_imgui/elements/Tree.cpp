//
// Created by petr on 10/31/20.
//

#include "Tree.h"
#include <imgui.h>

namespace pf::ui::ig {

Tree::Tree(const std::string &elementName, const std::string &label) : Element(elementName), Labellable(label) {}

Tree &Tree::addNode(const std::string &elementName, const std::string &caption) {
  return createChild<Tree>(elementName, caption);
}

void Tree::renderImpl() {
  if (ImGui::TreeNode(getLabel().c_str())) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
    ImGui::TreePop();
  }
}

}// namespace pf::ui::ig