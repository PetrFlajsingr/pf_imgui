//
// Created by xflajs00 on 25.01.2021.
//

#include "AbsoluteLayout.h"

namespace pf::ui::ig {

AbsoluteLayout::AbsoluteLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
                               ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent) {}

AbsoluteLayout::AbsoluteLayout(const std::string &elementName, const Size &size, ShowBorder showBorder,
                               Persistent persistent)
    : AbsoluteLayout(elementName, size, AllowCollapse::No, showBorder, persistent) {}

AbsoluteLayout::AbsoluteLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
                               Persistent persistent)
    : AbsoluteLayout(elementName, size, allowCollapse, ShowBorder::No, persistent) {}

void AbsoluteLayout::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto flags = isScrollable() ? ImGuiWindowFlags_HorizontalScrollbar
                                    : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{[&] { ImGui::EndChild(); }};
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      std::ranges::for_each(children, [](auto &childPair) {
        auto &[child, positionable] = childPair;
        ImGui::SetCursorPos(positionable->getPosition().asImVec());
        child->render();
      });
    }
  }
}
void AbsoluteLayout::setChildPosition(const std::string &childName, Position position) {
  if (auto child = findIf(children | ranges::views::addressof,
                          [childName](auto child) { return child->first->getName() == childName; });
      child.has_value()) {
    child.value()->second->setPosition(position);
  } else {
#ifndef _MSC_VER// TODO: MSVC internal compiler error
    throw IdNotFoundException("Child not found: {}", childName);
#endif
  }
}
void AbsoluteLayout::removeChild(const std::string &childName) {
  if (auto iter = std::ranges::find_if(children,
                                       [childName](const auto &child) { return child.first->getName() == childName; });
      iter != children.end()) {
    children.erase(iter);
  }
}
std::vector<Renderable *> AbsoluteLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.first.get(); })
      | ranges::to_vector;
}
}// namespace pf::ui::ig