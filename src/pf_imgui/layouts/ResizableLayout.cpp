//
// Created by petr on 1/24/21.
//

#include "ResizableLayout.h"

namespace pf::ui::ig {

ResizableLayout::ResizableLayout(const std::string &elementName, const ImVec2 &size, AllowCollapse allowCollapse, ShowBorder showBorder,
                                 Persistent persistent)
    : Layout(elementName, allowCollapse, showBorder, persistent), Resizable(size) {}


ImVec2 ResizableLayout::getSizeIfCollapsed() const {
  return isCollapsed() ? ImVec2{getSize().x, 20} : getSize();
}

}// namespace pf::ui::ig
