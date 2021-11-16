//
// Created by petr on 10/31/20.
//

#include "Checkbox.h"
#include <imgui.h>
#include <pf_imgui/details/ToggleButton.h>

namespace pf::ui::ig {

Checkbox::Checkbox(const std::string &elementName, const std::string &label, bool value, Persistent persistent)
    : Checkbox(elementName, label, Type::Checkbox, value, persistent) {}

Checkbox::Checkbox(const std::string &elementName, const std::string &label, Checkbox::Type checkboxType, bool value,
                   Persistent persistent)
    : ItemElement(elementName), ValueObservable(value), Labellable(label), Savable(persistent), type(checkboxType) {}

void Checkbox::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  bool valueChanged = false;
  switch (type) {
    case Type::Checkbox: valueChanged = ImGui::Checkbox(getLabel().c_str(), getValueAddress()); break;
    case Type::Toggle:
      ImGui::Text("%s", getLabel().c_str());
      ImGui::SameLine();
      valueChanged = ToggleButton(getLabel().c_str(), getValueAddress());
      break;
  }
  if (valueChanged) { notifyValueChanged(); }
}

void Checkbox::unserialize_impl(const toml::table &src) {
  if (auto newValIter = src.find("checked"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setValueAndNotifyIfChanged(*newVal); }
  }
}

toml::table Checkbox::serialize_impl() { return toml::table{{{"checked", getValue()}}}; }

void Checkbox::setSelected(bool selected) { setValueAndNotifyIfChanged(selected); }

bool Checkbox::isSelected() const { return getValue(); }

void Checkbox::toggle() { setSelected(!isSelected()); }

}// namespace pf::ui::ig