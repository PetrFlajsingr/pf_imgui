//
// Created by petr on 1/24/21.
//

#include "Layout.h"

namespace pf::ui::ig {

Layout::Layout(const std::string &elementName, const ImVec2 &size, bool showBorder)
    : Element(elementName), Resizable(size), drawBorder(showBorder) {}

bool Layout::isDrawBorder() const { return drawBorder; }
void Layout::setDrawBorder(bool showBorder) { drawBorder = showBorder; }

}// namespace pf::ui::ig
