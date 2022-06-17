//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Element::Element(const std::string &name) : ElementBase(name) {}

void Element::setFont(Font newFont) { font = newFont; }  //-V813

Font Element::getFont() const { return font; }

ImGuiID Element::getId() const { return id; }

void Element::setId(ImGuiID newId) { id = newId; }

void Element::render() {
  setId(ImGui::GetID(getName().c_str()));
  ImGui::PushOverrideID(id);
  RAII end{[&] { ImGui::PopID(); }};
  [[maybe_unused]] auto scopedFont = font.applyScopedIfNotDefault();
  ElementBase::render();
}

}  // namespace pf::ui::ig
