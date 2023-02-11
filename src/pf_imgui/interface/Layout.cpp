//
// Created by petr on 1/25/21.
//

#include "Layout.h"
#include <string>

namespace pf::ui::ig {

Layout::Layout(std::string_view elementName, const Size &initialSize, ShowBorder showBorder)
    : ElementWithID(elementName), size(initialSize), drawBorder(showBorder == ShowBorder::Yes) {}

bool Layout::isDrawBorder() const { return drawBorder; }

void Layout::setDrawBorder(bool showBorder) { drawBorder = showBorder; }

bool Layout::isScrollable() const { return scrollable; }

void Layout::setScrollable(bool scroll) { scrollable = scroll; }

void Layout::setScrollPosition(Layout::ScrollPosition position) { nextFrameScrollPosition = position; }

bool Layout::isHorizontalScrollEnabled() const { return horizontalScrollEnabled; }

void Layout::setHorizontalScrollEnabled(bool horizontalScroll) { horizontalScrollEnabled = horizontalScroll; }

}  // namespace pf::ui::ig
