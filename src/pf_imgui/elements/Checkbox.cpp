//
// Created by petr on 10/31/20.
//

#include "Checkbox.h"
#include <imgui.h>
#include <pf_imgui/details/ToggleButton.h>
#include <toml++/toml_node_view.h>
namespace pf::ui::ig {

Checkbox::Checkbox(const std::string &elementName, const std::string &label, Persistent persistent, bool value)
    : Checkbox(elementName, label, Type::Checkbox, persistent, value) {}

Checkbox::Checkbox(const std::string &elementName, const std::string &label, Checkbox::Type checkboxType,
                   Persistent persistent, bool value)
    : ItemElement(elementName), ValueObservable(value), Labellable(label), Savable(persistent), type(checkboxType) {}

void Checkbox::renderImpl() {
  bool valueChanged;
  switch (type) {
    case Type::Checkbox: valueChanged = ImGui::Checkbox(getLabel().c_str(), getValueAddress()); break;
    case Type::Toggle: valueChanged = ToggleButton(getLabel().c_str(), getValueAddress()); break;
  }
  if (valueChanged) { notifyValueChanged(); }
}

void Checkbox::unserialize_impl(const toml::table &src) { setValueAndNotifyIfChanged(src["checked"].as_boolean()); }

toml::table Checkbox::serialize_impl() { return toml::table{{{"checked", getValue()}}}; }

void Checkbox::setSelected(bool value) { setValueAndNotifyIfChanged(value); }

bool Checkbox::isSelected() const { return getValue(); }
void Checkbox::toggle() { setSelected(!isSelected()); }

}// namespace pf::ui::ig