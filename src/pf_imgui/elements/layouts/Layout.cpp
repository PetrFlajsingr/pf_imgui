//
// Created by petr on 1/24/21.
//

#include "Layout.h"

namespace pf::ui::ig {

Layout::Layout(const std::string &elementName, const ImVec2 &size) : Element(elementName), Resizable(size) {}

}// namespace pf::ui::ig
