//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>
#include <pf_imgui/FontManager.h>
#include <utility>

namespace pf::ui::ig {
Element::Element(const std::string &name) : Renderable(name) {}
void Element::render() {
  ImGui::PushID(getName().c_str());
  ImGui::PushFont(font);
  Renderable::render();
  ImGui::PopFont();
  ImGui::PopID();
}
void Element::setFont(const std::string &fontName) {
  if (auto newFont = fontManager->fontByName(fontName); newFont.has_value()) { font = *newFont; }
}
void Element::setFont(ImFont *fontPtr) { font = fontPtr; }
}// namespace pf::ui::ig
