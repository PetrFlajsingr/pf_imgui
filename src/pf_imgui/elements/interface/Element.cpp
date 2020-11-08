//
// Created by petr on 10/31/20.
//

#include "Element.h"

namespace pf::ui::ig {

Element::Element(std::string elementName) : name(std::move(elementName)) {}

Element::Element(Element &&other) noexcept {
  if (isMoved) {
    return;
  }
  name = std::move(other.name);
  visibility = other.visibility;
  isMoved = true;
}

Element &Element::operator=(Element &&other) noexcept {
  if (isMoved) {
    return *this;
  }
  name = std::move(other.name);
  visibility = other.visibility;
  isMoved = true;
  return *this;
}

const std::string &Element::getName() const { return name; }

Visibility Element::getVisibility() const { return visibility; }

void Element::setVisibility(Visibility visi) { visibility = visi; }

void Element::render() {
  if (visibility == Visibility::Visible) {
    renderImpl();
  }
}

}// namespace pf::ui::ig