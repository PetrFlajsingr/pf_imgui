//
// Created by petr.flajsingr on 2/9/2022.
//

#include "WrapLayout.h"

namespace pf::ui::ig {

WrapLayout::WrapLayout(WrapLayout::Config &&config)
    : WrapLayout(config.name, config.layoutDirection, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No) {}

WrapLayout::WrapLayout(std::string_view elementName, LayoutDirection layoutDirection, Size initialSize, ShowBorder showBorder)
    : LinearLayout(elementName, initialSize, showBorder), direction(layoutDirection) {}

void WrapLayout::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto flags = isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  ScopeExit end{&ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(*size), isDrawBorder(), flags)) {
    auto scrollApplier = applyScroll();
    if (dimensionPreviousFrame.size() != getChildren().size()) {
      dimensionPreviousFrame.clear();
      dimensionPreviousFrame.resize(getChildren().size());
      dimensionsCalculated = false;
    }
    switch (direction) {
      case LayoutDirection::LeftToRight: renderLeftToRight(); break;
      case LayoutDirection::TopToBottom: renderTopToBottom(); break;
    }
  }
}

void WrapLayout::renderLeftToRight() {
  auto childrenView = getChildren();
  if (!childrenView.empty()) {
    const auto childrenSize = childrenView.size();
    std::size_t idx{};
    const auto windowVisibleX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    const auto &imStyle = ImGui::GetStyle();
    std::ranges::for_each(childrenView, [&](auto &child) {
      child.render();
      const auto lastOriginX = ImGui::GetItemRectMin().x;
      const auto lastX = ImGui::GetItemRectMax().x;
      if (dimensionsCalculated && idx != childrenSize - 1) {
        const auto nextXH = lastX + imStyle.ItemSpacing.x + dimensionPreviousFrame[idx + 1];
        if (idx != childrenSize - 1 && nextXH < windowVisibleX) { ImGui::SameLine(); }
      } else {
        const auto nextX = lastX + imStyle.ItemSpacing.x + (lastX - lastOriginX);
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
    const auto &imStyle = ImGui::GetStyle();
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
          currentX = maxXInColumn + imStyle.ItemSpacing.x;
          ImGui::SetCursorPosY(0);
        }
      } else {
        const auto nextY = lastY + imStyle.ItemSpacing.y + (lastY - lastOriginY);
        if (nextY >= windowVisibleY) {
          currentX = maxXInColumn + imStyle.ItemSpacing.x;
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
