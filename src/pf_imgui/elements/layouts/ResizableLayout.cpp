//
// Created by petr on 1/24/21.
//

#include "ResizableLayout.h"

namespace pf::ui::ig {

ResizableLayout::ResizableLayout(const std::string &elementName, const ImVec2 &size, AllowCollapse allowCollapse,
                                 ShowBorder showBorder)
    : Layout(elementName, allowCollapse, showBorder), Resizable(size) {}

ImVec2 ResizableLayout::getSizeIfCollapsed() const {
  return isCollapsed() ? ImVec2{getSize().x, 30} : getSize();
}

}// namespace pf::ui::ig
