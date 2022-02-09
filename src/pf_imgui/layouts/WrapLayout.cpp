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
      const auto lastOriginX = ImGui::GetItemRectMin().x;
      const auto lastX = ImGui::GetItemRectMax().x;
      const auto nextX = lastX + style.ItemSpacing.x + (lastX - lastOriginX);
      if (idx != childrenSize - 1 && nextX < windowVisibleX) { ImGui::SameLine(); }
      ++idx;
    });
  }
}

void WrapLayout::renderTopToBottom() {
  auto childrenView = getChildren();
  if (!childrenView.empty()) {
    float currentX = 0;
    float maxXInColumn = currentX;
    const auto windowVisibleY = ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y;
    const auto &style = ImGui::GetStyle();
    std::ranges::for_each(childrenView, [&](auto &child) {
      ImGui::SetCursorPosX(currentX);
      child.render();
      maxXInColumn = std::max(maxXInColumn, ImGui::GetItemRectMax().x - ImGui::GetWindowPos().x);
      const auto lastOriginY = ImGui::GetItemRectMin().y;
      const auto lastY = ImGui::GetItemRectMax().y;
      const auto nextY = lastY + style.ItemSpacing.y + (lastY - lastOriginY);
      if (nextY >= windowVisibleY) {
        currentX = maxXInColumn + style.ItemSpacing.x;
        ImGui::SetCursorPosY(0);
      }
    });
  }
}

}  // namespace pf::ui::ig