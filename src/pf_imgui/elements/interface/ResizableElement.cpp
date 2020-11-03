//
// Created by petr on 10/31/20.
//

#include "ResizableElement.h"
#include <utility>

namespace pf::ui::ig {

ResizableElement::ResizableElement(std::string elementName, const ImVec2 &size)
    : Element(std::move(elementName)), size(size) {}

const ImVec2 &ResizableElement::getSize() const { return size; }

void ResizableElement::setSize(const ImVec2 &s) { size = s; }
}// namespace pf::ui