//
// Created by petr on 10/31/20.
//

#include "Checkbox.h"
#include <imgui.h>
#include <toml++/toml_node_view.h>

namespace pf::ui::ig {

Checkbox::Checkbox(const std::string &elementName, const std::string &label, Persistent persistent, bool value)
    : ItemElement(elementName), ValueObservable(value), Labellable(label), Savable(persistent) {}

void Checkbox::renderImpl() {
  if (ImGui::Checkbox(getLabel().c_str(), getValueAddress())) { notifyValueChanged(); }
}

void Checkbox::unserialize_impl(const toml::table &src) { setValueAndNotifyIfChanged(src["checked"].as_boolean()); }

toml::table Checkbox::serialize_impl() { return toml::table{{{"checked", getValue()}}}; }

void Checkbox::setSelected(bool value) { setValueAndNotifyIfChanged(value); }

bool Checkbox::isSelected() const { return getValue(); }

}// namespace pf::ui::ig