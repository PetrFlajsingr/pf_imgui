//
// Created by petr on 1/24/21.
//

#include "BoxLayout.h"
#include <imgui.h>
#include <numeric>
#include <range/v3/range/conversion.hpp>
#include <utility>

namespace pf::ui::ig {

BoxLayout::BoxLayout(BoxLayout::Config &&config)
    : ResizableLayout(std::string{config.name}, config.size, config.allowCollapse, config.showBorder,
                      config.persistent),
      layoutDirection(config.layoutDirection) {}

BoxLayout::BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
                     AllowCollapse allowCollapse, ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent), layoutDirection(layoutDirection) {}

BoxLayout::BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
                     ShowBorder showBorder, Persistent persistent)
    : BoxLayout(elementName, layoutDirection, size, AllowCollapse::No, showBorder, persistent) {}

BoxLayout::BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
                     AllowCollapse allowCollapse, Persistent persistent)
    : BoxLayout(elementName, layoutDirection, size, allowCollapse, ShowBorder::No, persistent) {}

LayoutDirection BoxLayout::getLayoutDirection() const { return layoutDirection; }
void BoxLayout::setLayoutDirection(LayoutDirection newLayoutDirection) { layoutDirection = newLayoutDirection; }

void BoxLayout::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (nextFrameScrollPosition.has_value() && *nextFrameScrollPosition == ScrollPosition::Top) {
      ImGui::SetScrollHereY(0.0f);
      nextFrameScrollPosition = std::nullopt;
    }
    if (renderCollapseButton()) {
      switch (layoutDirection) {
        case LayoutDirection::LeftToRight: renderLeftToRight(); break;
        case LayoutDirection::TopToBottom: renderTopToBottom(); break;
      }
    }
    if (nextFrameScrollPosition.has_value() && *nextFrameScrollPosition == ScrollPosition::Bottom) {
      ImGui::SetScrollHereY(1.0f);
      nextFrameScrollPosition = std::nullopt;
    }
  }
}

void BoxLayout::renderTopToBottom() {
  std::ranges::for_each(children, [&](auto &child) { child->render(); });
}

void BoxLayout::renderLeftToRight() {
  if (!children.empty()) {
    const auto childrenSize = children.size();
    auto idx = 0u;
    std::ranges::for_each(children, [&](auto &child) {
      child->render();
      if (idx != childrenSize - 1) { ImGui::SameLine(); }
      ++idx;
    });
  }
}

void BoxLayout::pushChild(std::unique_ptr<Element> child) { children.emplace_back(std::move(child)); }
void BoxLayout::insertChild(std::unique_ptr<Element> child, std::size_t index) {
#ifndef _MSC_VER  // TODO: MSVC internal error
  if (index > children.size()) { throw InvalidArgumentException("Index out of bounds: {}", index); }
#endif
  children.insert(children.begin() + static_cast<long long>(index), std::move(child));
}
void BoxLayout::removeChild(const std::string &childName) {
  if (auto iter =
          std::ranges::find_if(children, [childName](const auto &child) { return child->getName() == childName; });
      iter != children.end()) {
    children.erase(iter);
  }
}
std::vector<Renderable *> BoxLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); })
      | ranges::to_vector;
}

}  // namespace pf::ui::ig
