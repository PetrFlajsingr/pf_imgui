//
// Created by petr on 1/25/21.
//

#include "Layout.h"

namespace pf::ui::ig {

Layout::Layout(const std::string &elementName, AllowCollapse allowCollapse, ShowBorder showBorder)
    : Element(elementName), Collapsible(allowCollapse), drawBorder(showBorder == ShowBorder::Yes) {}

Layout::Layout(const std::string &elementName, ShowBorder showBorder)
    : Layout(elementName, AllowCollapse::No, showBorder) {}
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
  return isCollapsed();
}

}// namespace pf::ui::ig
