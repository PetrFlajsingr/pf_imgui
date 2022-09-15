//
// Created by petr on 10/31/20.
//

#include "ElementWithID.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

ElementWithID::ElementWithID(std::string_view elementName) : Element(elementName) {}  //-V730

ImGuiID ElementWithID::getId() const { return id; }

void ElementWithID::setId(ImGuiID newId) { id = newId; }

void ElementWithID::render() {
  // TODO: do this only once?
  setId(ImGui::GetID(getName().c_str()));
  ImGui::PushOverrideID(id);
  RAII end{[&] { ImGui::PopID(); }};
  Element::render();
}

}  // namespace pf::ui::ig
