//
// Created by petr on 1/25/21.
//

#include "Layout.h"
#include <string>

namespace pf::ui::ig {

Layout::Layout(const std::string &elementName, const Size &size, ShowBorder showBorder)
    : ElementWithID(elementName), Resizable(size), drawBorder(showBorder == ShowBorder::Yes) {}

bool Layout::isDrawBorder() const { return drawBorder; }

void Layout::setDrawBorder(bool showBorder) { drawBorder = showBorder; }

bool Layout::isScrollable() const { return scrollable; }

void Layout::setScrollable(bool scroll) { scrollable = scroll; }

void Layout::setScrollPosition(Layout::ScrollPosition position) { nextFrameScrollPosition = position; }

bool Layout::isHorizontalScrollEnabled() const { return horizontalScrollEnabled; }

void Layout::setHorizontalScrollEnabled(bool horizontalScroll) { horizontalScrollEnabled = horizontalScroll; }

RAII Layout::applyScroll() {
  if (nextFrameScrollPosition.has_value() && *nextFrameScrollPosition == ScrollPosition::Top) {
    ImGui::SetScrollHereY(0.0f);
    nextFrameScrollPosition.reset();
  }
  return RAII{[this] {
    if (nextFrameScrollPosition.has_value() && *nextFrameScrollPosition == ScrollPosition::Bottom) {
      ImGui::SetScrollHereY(1.0f);
      nextFrameScrollPosition.reset();
    }
  }};
}

}  // namespace pf::ui::ig
