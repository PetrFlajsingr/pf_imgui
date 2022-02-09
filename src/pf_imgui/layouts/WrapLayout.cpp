//
// Created by petr.flajsingr on 2/9/2022.
//

#include "WrapLayout.h"

namespace pf::ui::ig {

WrapLayout::WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
                       AllowCollapse allowCollapse, ShowBorder showBorder, Persistent persistent)
    : BoxLayout(elementName, layoutDirection, size, allowCollapse, showBorder, persistent) {}

WrapLayout::WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
                       ShowBorder showBorder, Persistent persistent)
    : BoxLayout(elementName, layoutDirection, size, showBorder, persistent) {}

WrapLayout::WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
                       AllowCollapse allowCollapse, Persistent persistent)
    : BoxLayout(elementName, layoutDirection, size, allowCollapse, persistent) {}

void WrapLayout::renderImpl() {
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
      switch (getLayoutDirection()) {
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
void WrapLayout::renderLeftToRight() {
  auto childrenView = getChildren();
  if (!childrenView.empty()) {
    const auto childrenSize = childrenView.size();
    auto idx = 0u;
    const auto windowVisibleX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    const auto &style = ImGui::GetStyle();
    std::ranges::for_each(childrenView, [&](auto &child) {
      child.render();
      const auto lastButtonOriginX = ImGui::GetItemRectMin().x;
      const auto lastButtonX = ImGui::GetItemRectMax().x;
      const auto nextButtonX = lastButtonX + style.ItemSpacing.x + (lastButtonX - lastButtonOriginX);
      if (idx != childrenSize - 1 && nextButtonX < windowVisibleX) { ImGui::SameLine(); }
      ++idx;
    });
  }
}

void WrapLayout::renderTopToBottom() {}

}