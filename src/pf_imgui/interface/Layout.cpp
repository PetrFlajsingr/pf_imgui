//
// Created by petr on 1/25/21.
//

#include "Layout.h"
#include <string>

namespace pf::ui::ig {

Layout::Layout(const std::string &elementName, AllowCollapse allowCollapse, ShowBorder showBorder,
               Persistent persistent)
    : Element(elementName), Collapsible(allowCollapse, persistent), drawBorder(showBorder == ShowBorder::Yes) {}

bool Layout::isDrawBorder() const { return drawBorder; }

void Layout::setDrawBorder(bool showBorder) { drawBorder = showBorder; }

bool Layout::isScrollable() const { return scrollable; }

void Layout::setScrollable(bool scroll) { scrollable = scroll; }

bool Layout::renderCollapseButton() {
  if (isCollapsible()) {
    const auto width = ImGui::GetContentRegionAvail().x;
    const auto btnPos = ImVec2{width - 10, 0};
    const auto btnDir = isCollapsed() ? ImGuiDir_::ImGuiDir_Down : ImGuiDir_::ImGuiDir_Up;
    const auto origDrawPos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(btnPos);
    RAII resetCursorPos{[&] { ImGui::SetCursorPos(origDrawPos); }};
    if (ImGui::ArrowButton("collapse_btn", btnDir)) { setCollapsed(!isCollapsed()); }
  }
  return !isCollapsed();
}
void Layout::setScrollPosition(Layout::ScrollPosition position) { nextFrameScrollPosition = position; }

bool Layout::isHorizontalScrollEnabled() const { return horizontalScrollEnabled; }

void Layout::setHorizontalScrollEnabled(bool horizontalScroll) { horizontalScrollEnabled = horizontalScroll; }

}  // namespace pf::ui::ig
