//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Element::Element(const std::string &name) : Renderable(name) {}

Element::~Element() { observableDestroy.notify(); }

Element::Element(Element &&other) noexcept
    : Renderable(std::move(other)), observableDestroy(std::move(other.observableDestroy)) {}

Element &Element::operator=(Element &&other) noexcept {
  observableDestroy = std::move(other.observableDestroy);
  Renderable::operator=(std::move(other));
  return *this;
}

void Element::render() {
  ImGui::PushID(getName().c_str());
  RAII end{[&] { ImGui::PopID(); }};
  auto fontScoped = font.applyScopedIfNotDefault();
  Renderable::render();
}

void Element::setFont(Font newFont) { font = newFont; }

Font Element::getFont() const { return font; }

}  // namespace pf::ui::ig
