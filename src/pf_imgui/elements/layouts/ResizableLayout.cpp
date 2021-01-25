//
// Created by petr on 1/24/21.
//

#include "ResizableLayout.h"

namespace pf::ui::ig {

ResizableLayout::ResizableLayout(const std::string &elementName, const ImVec2 &size, bool showBorder)
    : Layout(elementName, showBorder), Resizable(size) {}
}// namespace pf::ui::ig
