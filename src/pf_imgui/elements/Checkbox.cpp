//
// Created by petr on 10/31/20.
//

#include "Checkbox.h"
#include <imgui.h>
#include <toml++/toml_node.hpp>

namespace pf::ui::ig {

Checkbox::Checkbox(const std::string &elementName, const std::string &caption,
                   Persistent persistent, bool value)
    : Element(elementName), ValueObservableElement(elementName, value),
      LabeledElement(elementName, caption), SavableElement(elementName, persistent) {}

void Checkbox::renderImpl() {
  const auto oldValue = getValue();
  ImGui::Checkbox(getLabel().c_str(), getValueAddress());
  if (getValue() != oldValue) { notifyValueChanged(); }
}

void Checkbox::unserialize_impl(const toml::table &src) {
  ;
  setValueAndNotifyIfChanged(src["checked"].as_boolean());
}

toml::table Checkbox::serialize_impl() { return toml::table{{{"checked", getValue()}}}; }

}// namespace pf::ui::ig