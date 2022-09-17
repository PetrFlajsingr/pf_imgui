//
// Created by Petr on 6/17/2022.
//

#include "Element.h"
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Element::Element(std::string_view elementName) : Renderable(elementName) {}

Element::~Element() { Event_notify(destroyEvent); }

Element::Element(Element &&other) noexcept  //-V730
    : Renderable(std::move(other)), destroyEvent(std::move(other.destroyEvent)) {}

Element &Element::operator=(Element &&other) noexcept {
  destroyEvent = std::move(other.destroyEvent);
  Renderable::operator=(std::move(other));
  return *this;
}

}  // namespace pf::ui::ig