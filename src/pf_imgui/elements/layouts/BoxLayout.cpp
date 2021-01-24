//
// Created by petr on 1/24/21.
//

#include "BoxLayout.h"
#include <imgui.h>
#include <numeric>

namespace pf::ui::ig {

BoxLayout::BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const ImVec2 &size,
                     bool showBorder)
    : Layout(elementName, size), layoutDirection(layoutDirection), showBorder(showBorder) {}

LayoutDirection BoxLayout::getLayoutDirection() const { return layoutDirection; }
void BoxLayout::setLayoutDirection(LayoutDirection newLayoutDirection) { layoutDirection = newLayoutDirection; }

void BoxLayout::renderImpl() {
  ImGui::BeginChild(getName().c_str(), getSize(), showBorder);
  switch (layoutDirection) {
    case LayoutDirection::LeftToRight: renderLeftToRight(); break;
    case LayoutDirection::TopToBottom: renderTopToBottom(); break;
  }
  ImGui::EndChild();
}

bool BoxLayout::isShowBorder() const { return showBorder; }

void BoxLayout::setShowBorder(bool border) { showBorder = border; }
void BoxLayout::renderTopToBottom() {
  std::ranges::for_each(children, [&](auto &child) { child->render(); });
}
void BoxLayout::renderLeftToRight() {
  if (!children.empty()) {
    const auto size = children.size();
    auto idx = 0u;
    std::ranges::for_each(children, [&](auto &child) {
      child->render();
      if (idx != size - 1) { ImGui::SameLine(); }
      ++idx;
    });
  }
}

}// namespace pf::ui::ig