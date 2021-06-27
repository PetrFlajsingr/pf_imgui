//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace pf::ui::ig {
Element::Element(const std::string &name) : Renderable(name) {}

void Element::render() {
  ImGui::PushID(getName().c_str());
  if (font != nullptr) { ImGui::PushFont(font); }
  Renderable::render();
  if (font != nullptr) { ImGui::PopFont(); }
  ImGui::PopID();
}

void Element::setFont(ImFont *fontPtr) { font = fontPtr; }
ImFont *Element::getFont() const {
  return font;
}

}// namespace pf::ui::ig
