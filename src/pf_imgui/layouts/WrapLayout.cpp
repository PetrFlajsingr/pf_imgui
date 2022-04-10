//
// Created by petr.flajsingr on 2/9/2022.
//

#include "WrapLayout.h"

namespace pf::ui::ig {

WrapLayout::WrapLayout(WrapLayout::Config &&config)
    : BoxLayout(std::string{config.name}, config.layoutDirection, config.size, config.showBorder) {}

WrapLayout::WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
                       ShowBorder showBorder)
    : BoxLayout(elementName, layoutDirection, size, showBorder) {}

void WrapLayout::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(getSize()), isDrawBorder(), flags)) {
    if (nextFrameScrollPosition.has_value() && *nextFrameScrollPosition == ScrollPosition::Top) {
      ImGui::SetScrollHereY(0.0f);
      nextFrameScrollPosition = std::nullopt;
    }
    if (dimensionPreviousFrame.size() != getChildren().size()) {
      dimensionPreviousFrame.clear();
      dimensionPreviousFrame.resize(getChildren().size());
      dimensionsCalculated = false;
    }
    switch (getLayoutDirection()) {
      case LayoutDirection::LeftToRight: renderLeftToRight(); break;
      case LayoutDirection::TopToBottom: renderTopToBottom(); break;
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
    std::size_t idx{};
    const auto windowVisibleX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    const auto &style = ImGui::GetStyle();
    std::ranges::for_each(childrenView, [&](auto &child) {
      child.render();
      const auto lastOriginX = ImGui::GetItemRectMin().x;
      const auto lastX = ImGui::GetItemRectMax().x;
      if (dimensionsCalculated && idx != childrenSize - 1) {
        const auto nextXH = lastX + style.ItemSpacing.x + dimensionPreviousFrame[idx + 1];
        if (idx != childrenSize - 1 && nextXH < windowVisibleX) { ImGui::SameLine(); }
      } else {
        const auto nextX = lastX + style.ItemSpacing.x + (lastX - lastOriginX);
        if (idx != childrenSize - 1 && nextX < windowVisibleX) { ImGui::SameLine(); }
      }
      dimensionPreviousFrame[idx] = ImGui::GetItemRectSize().x;
      ++idx;
    });
    dimensionsCalculated = true;
  }
}

void WrapLayout::renderTopToBottom() {
  auto childrenView = getChildren();
  if (!childrenView.empty()) {
    const auto childrenSize = childrenView.size();
    float currentX = 0;
    float maxXInColumn = currentX;
    const auto windowVisibleY = ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y;
    const auto &style = ImGui::GetStyle();
    std::size_t idx{};
    std::ranges::for_each(childrenView, [&](auto &child) {
      ImGui::SetCursorPosX(currentX);
      child.render();
      maxXInColumn = std::max(maxXInColumn, ImGui::GetItemRectMax().x - ImGui::GetWindowPos().x);
      const auto lastOriginY = ImGui::GetItemRectMin().y;
      const auto lastY = ImGui::GetItemRectMax().y;
      if (dimensionsCalculated && idx != childrenSize - 1) {
        const auto nextYH = lastY + dimensionPreviousFrame[idx + 1];
        if (nextYH >= windowVisibleY) {
          currentX = maxXInColumn + style.ItemSpacing.x;
          ImGui::SetCursorPosY(0);
        }
      } else {
        const auto nextY = lastY + style.ItemSpacing.y + (lastY - lastOriginY);
        if (nextY >= windowVisibleY) {
          currentX = maxXInColumn + style.ItemSpacing.x;
          ImGui::SetCursorPosY(0);
        }
      }
      dimensionPreviousFrame[idx] = ImGui::GetItemRectSize().y;
      ++idx;
    });
    dimensionsCalculated = true;
  }
}

}  // namespace pf::ui::ig
