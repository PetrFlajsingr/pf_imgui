//
// Created by Petr on 6/17/2022.
//

#include "Element.h"
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Element::Element(const std::string &elementName) : Renderable(elementName) {}

Element::~Element() { observableDestroy.notify(); }

Element::Element(Element &&other) noexcept  //-V730
    : Renderable(std::move(other)), observableDestroy(std::move(other.observableDestroy)) {}

Element &Element::operator=(Element &&other) noexcept {
  observableDestroy = std::move(other.observableDestroy);
  Renderable::operator=(std::move(other));
  return *this;
}

}  // namespace pf::ui::ig