//
// Created by petr on 1/24/21.
//

#include "ResizableLayout.h"

namespace pf::ui::ig {

ResizableLayout::ResizableLayout(const std::string &elementName, const ImVec2 &size, AllowCollapse allowCollapse,
                                 ShowBorder showBorder)
    : Layout(elementName, allowCollapse, showBorder), Resizable(size) {}

ImVec2 ResizableLayout::getSizeIfCollapsed() const {
  return isCollapsed() ? getSize() : ImVec2{getSize().x, 10};
}

}// namespace pf::ui::ig
