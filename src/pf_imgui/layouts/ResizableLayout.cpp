//
// Created by petr on 1/24/21.
//

#include "ResizableLayout.h"

namespace pf::ui::ig {

ResizableLayout::ResizableLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
                                 ShowBorder showBorder, Persistent persistent)
    : Layout(elementName, allowCollapse, showBorder, persistent), Resizable(size) {}

ImVec2 ResizableLayout::getSizeIfCollapsed() const {
  return isCollapsed() ? ImVec2{getSize().width.value, 20} : getSize().asImVec();
}

}  // namespace pf::ui::ig
