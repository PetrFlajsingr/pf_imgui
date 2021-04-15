//
// Created by xflajs00 on 25.01.2021.
//

#include "AbsoluteLayout.h"

namespace pf::ui::ig {

AbsoluteLayout::AbsoluteLayout(const std::string &elementName, const ImVec2 &size, AllowCollapse allowCollapse,
                               ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent) {}

AbsoluteLayout::AbsoluteLayout(const std::string &elementName, const ImVec2 &size, ShowBorder showBorder,
                               Persistent persistent)
    : AbsoluteLayout(elementName, size, AllowCollapse::No, showBorder, persistent) {}

AbsoluteLayout::AbsoluteLayout(const std::string &elementName, const ImVec2 &size, AllowCollapse allowCollapse,
                               Persistent persistent)
    : AbsoluteLayout(elementName, size, allowCollapse, ShowBorder::No, persistent) {}

void AbsoluteLayout::renderImpl() {
  const auto flags = isScrollable() ? ImGuiWindowFlags_HorizontalScrollbar
                                    : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      std::ranges::for_each(children, [](auto &childPair) {
        auto &[child, positionable] = childPair;
        [[maybe_unused]] auto hihi = positionable->getPosition();
        ImGui::SetCursorPos(positionable->getPosition());
        child->render();
      });
    }
  }
  ImGui::EndChild();
}
void AbsoluteLayout::setChildPosition(const std::string &name, ImVec2 position) {
  if (auto child =
          findIf(children | ranges::views::addressof, [name](auto child) { return child->first->getName() == name; });
      child.has_value()) {
    child.value()->second->setPosition(position);
  } else {
    throw IdNotFoundException("Child not found: {}", name);
  }
}
void AbsoluteLayout::removeChild(const std::string &name) {
  if (auto iter = std::ranges::find_if(children, [name](const auto &child) { return child.first->getName() == name; });
      iter != children.end()) {
    children.erase(iter);
  }
}
std::vector<Renderable *> AbsoluteLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.first.get(); })
      | ranges::to_vector;
}
}// namespace pf::ui::ig