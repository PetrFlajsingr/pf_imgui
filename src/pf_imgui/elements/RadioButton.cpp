//
// Created by petr on 10/31/20.
//

#include "RadioButton.h"
#include <imgui.h>

namespace pf::ui::ig {

RadioButton::RadioButton(RadioButton::Config &&config)
    : ItemElement(std::string{config.name.value}), ValueObservable(config.selected),
      label(std::string{config.label.value}) {}

RadioButton::RadioButton(const std::string &elementName, const std::string &labelText, bool initialValue)
    : ItemElement(elementName), ValueObservable(initialValue), label(labelText) {}

void RadioButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto oldValue = getValue();
  if (ImGui::RadioButton(label.get().c_str(), getValue())) { setValueInner(true); }
  if (oldValue != getValue()) { notifyValueChanged(); }
}

bool RadioButton::isSelected() const { return getValue(); }

}  // namespace pf::ui::ig
