//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <utility>
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Element::Element(std::string elementName) : name(std::move(elementName)) {}

Element::Element(Element &&other) noexcept : Renderable(std::move(other)), name(std::move(other.name)) {}

Element &Element::operator=(Element &&other) noexcept {
  name = std::move(other.name);
  Renderable::operator=(std::move(other));
  return *this;
}

const std::string &Element::getName() const { return name; }

void Element::render() {
  ImGui::PushID(getName().c_str());
  Renderable::render();
  ImGui::PopID();
}

}// namespace pf::ui::ig