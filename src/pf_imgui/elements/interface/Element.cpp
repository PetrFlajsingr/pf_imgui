//
// Created by petr on 10/31/20.
//

#include "Element.h"

namespace pf::ui::ig {

Element::Element(std::string elementName) : name(std::move(elementName)) {}

const std::string &Element::getName() const { return name; }

Visibility Element::getVisibility() const { return visibility; }
void Element::setVisibility(Visibility visi) { visibility = visi; }
void Element::render() {
  if (visibility == Visibility::Visible) {
    renderImpl();
  }
}

}