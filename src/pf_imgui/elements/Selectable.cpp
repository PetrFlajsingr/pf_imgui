//
// Created by petr on 5/22/21.
//

#include "Selectable.h"
#include <imgui.h>

namespace pf::ui::ig {
Selectable::Selectable(const std::string &elementName, const std::string &label, bool value, const Size &s,
                       Persistent persistent)
    : ItemElement(elementName), Labellable(label), ValueObservable(value), Resizable(s), Savable(persistent) {}

void Selectable::renderImpl() {
  if (ImGui::Selectable(getLabel().c_str(), getValueAddress(), 0, getSize().asImVec())) { notifyValueChanged(); }
}

void Selectable::unserialize_impl(const toml::table &src) { setValueAndNotifyIfChanged(src["selected"].as_boolean()); }

toml::table Selectable::serialize_impl() { return toml::table{{{"selected", getValue()}}}; }

}// namespace pf::ui::ig
