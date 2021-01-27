//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {
Element::Element(const std::string &name) : Renderable(name) {}
void Element::render() {
  ImGui::PushID(getName().c_str());
  Renderable::render();
  ImGui::PopID();
}
}// namespace pf::ui::ig