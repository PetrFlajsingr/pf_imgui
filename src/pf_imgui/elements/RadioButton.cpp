//
// Created by petr on 10/31/20.
//

#include "RadioButton.h"
#include <imgui.h>

namespace pf::ui::ig {

RadioButton::RadioButton(const std::string &elementName, const std::string &label, bool value)
    : ItemElement(elementName), Labellable(label), ValueObservable(value) {}

void RadioButton::renderImpl() {
  const auto oldValue = getValue();
  if (ImGui::RadioButton(getLabel().c_str(), getValue())) { setValue(true); }
  if (oldValue != getValue() && getValue()) { notifyValueChanged(); }
}

bool RadioButton::isSelected() const { return getValue(); }

}// namespace pf::ui::ig