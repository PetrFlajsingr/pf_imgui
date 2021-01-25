//
// Created by petr on 1/25/21.
//

#include "Layout.h"

namespace pf::ui::ig {

Layout::Layout(const std::string &elementName, bool showBorder) : Element(elementName), drawBorder(showBorder) {}

bool Layout::isDrawBorder() const { return drawBorder; }
void Layout::setDrawBorder(bool showBorder) { drawBorder = showBorder; }

bool Layout::isScrollable() const { return scrollable; }

void Layout::setScrollable(bool scroll) { scrollable = scroll; }

}// namespace pf::ui::ig
