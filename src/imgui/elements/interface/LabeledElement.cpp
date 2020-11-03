//
// Created by petr on 10/31/20.
//

#include "LabeledElement.h"

#include <utility>

namespace pf::ui::ig {
LabeledElement::LabeledElement(std::string elementName,
                                             std::string caption)
    : Element(std::move(elementName)), label(std::move(caption)) {}

const std::string &LabeledElement::getLabel() const { return label; }

void LabeledElement::setLabel(const std::string &cap) { label = cap;
}
}