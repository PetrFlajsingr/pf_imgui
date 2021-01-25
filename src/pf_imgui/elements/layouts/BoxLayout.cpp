//
// Created by petr on 1/24/21.
//

#include "BoxLayout.h"
#include <imgui.h>
#include <numeric>

namespace pf::ui::ig {

BoxLayout::BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const ImVec2 &size,
                     bool showBorder)
    : Layout(elementName, size, showBorder), layoutDirection(layoutDirection) {}

LayoutDirection BoxLayout::getLayoutDirection() const { return layoutDirection; }
void BoxLayout::setLayoutDirection(LayoutDirection newLayoutDirection) { layoutDirection = newLayoutDirection; }

void BoxLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSize(), isDrawBorder(), flags)) {
    switch (layoutDirection) {
      case LayoutDirection::LeftToRight: renderLeftToRight(); break;
      case LayoutDirection::TopToBottom: renderTopToBottom(); break;
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

}// namespace pf::ui::ig