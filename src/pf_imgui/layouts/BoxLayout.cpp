//
// Created by petr on 1/24/21.
//

#include "BoxLayout.h"
#include <imgui.h>
#include <numeric>

namespace pf::ui::ig {

BoxLayout::BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const ImVec2 &size,
                     AllowCollapse allowCollapse, ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent), layoutDirection(layoutDirection) {}

BoxLayout::BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const ImVec2 &size,
                     ShowBorder showBorder, Persistent persistent)
    : BoxLayout(elementName, layoutDirection, size, AllowCollapse::No, showBorder, persistent) {}

BoxLayout::BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const ImVec2 &size,
                     AllowCollapse allowCollapse, Persistent persistent)
    : BoxLayout(elementName, layoutDirection, size, allowCollapse, ShowBorder::No, persistent) {}

LayoutDirection BoxLayout::getLayoutDirection() const { return layoutDirection; }
void BoxLayout::setLayoutDirection(LayoutDirection newLayoutDirection) { layoutDirection = newLayoutDirection; }

void BoxLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      switch (layoutDirection) {
        case LayoutDirection::LeftToRight: renderLeftToRight(); break;
        case LayoutDirection::TopToBottom: renderTopToBottom(); break;
      }
    }
  }
  ImGui::EndChild();
}

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

void BoxLayout::pushChild(std::unique_ptr<Element> child) { children.emplace_back(std::move(child)); }
void BoxLayout::insertChild(std::unique_ptr<Element> child, std::size_t index) {
  if (index > children.size()) { throw StackTraceException::fmt("Index out of bounds: {}", index); }
  children.insert(children.begin() + index, std::move(child));
}
void BoxLayout::removeChild(const std::string &name) {
  if (auto iter = std::ranges::find_if(children, [name](const auto &child) { return child->getName() == name; });
      iter != children.end()) {
    children.erase(iter);
  }
}
std::vector<Renderable *> BoxLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); })
      | ranges::to_vector;
}

}// namespace pf::ui::ig