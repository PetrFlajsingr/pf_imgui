//
// Created by xflajs00 on 25.01.2021.
//

#include "AbsoluteLayout.h"
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

AbsoluteLayout::AbsoluteLayout(AbsoluteLayout::Config &&config)
    : ResizableLayout(std::string{config.name}, config.size, config.allowCollapse, config.showBorder,
                      config.persistent) {}

AbsoluteLayout::AbsoluteLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
                               ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent) {}

void AbsoluteLayout::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  const auto flags = isScrollable() ? ImGuiWindowFlags_HorizontalScrollbar
                                    : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      std::ranges::for_each(children, [](auto &childPair) {
        auto &[child, positionable] = childPair;
        ImGui::SetCursorPos(static_cast<ImVec2>(positionable->getPosition()));
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
#ifndef _MSC_VER  // TODO: MSVC internal compiler error
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

}  // namespace pf::ui::ig
