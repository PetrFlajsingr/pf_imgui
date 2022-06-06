//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Element::Element(const std::string &name) : Renderable(name) {} //-V730

Element::~Element() { observableDestroy.notify(); }

Element::Element(Element &&other) noexcept //-V730
    : Renderable(std::move(other)), observableDestroy(std::move(other.observableDestroy)) {}

Element &Element::operator=(Element &&other) noexcept {
  observableDestroy = std::move(other.observableDestroy);
  Renderable::operator=(std::move(other));
  return *this;
}

void Element::render() {
  setId(ImGui::GetID(getName().c_str()));
  ImGui::PushOverrideID(id);
  RAII end{[&] { ImGui::PopID(); }};
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  Renderable::render();
}

void Element::setFont(Font newFont) { font = newFont; } //-V813

Font Element::getFont() const { return font; }

ImGuiID Element::getId() const { return id; }

void Element::setId(ImGuiID newId) {
  id = newId;
}

}  // namespace pf::ui::ig
