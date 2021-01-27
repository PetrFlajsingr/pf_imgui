//
// Created by petr on 1/25/21.
//

#include "Layout.h"

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
    const auto width = ImGui::GetContentRegionAvailWidth();
    const auto btnPos = ImVec2{width - 10, 0};
    const auto btnDir = isCollapsed() ? ImGuiDir_::ImGuiDir_Down : ImGuiDir_::ImGuiDir_Up;
    const auto origDrawPos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(btnPos);
    if (ImGui::ArrowButton((getName() + "_collapse_btn").c_str(), btnDir)) { setCollapsed(!isCollapsed()); }
    ImGui::SetCursorPos(origDrawPos);
  }
  return !isCollapsed();
}

}// namespace pf::ui::ig
