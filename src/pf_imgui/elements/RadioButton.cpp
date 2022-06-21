//
// Created by petr on 10/31/20.
//

#include "RadioButton.h"
#include <imgui.h>

namespace pf::ui::ig {

RadioButton::RadioButton(RadioButton::Config &&config)
    : ItemElement(std::string{config.name.value}), Labellable(std::string{config.label.value}),
      ValueObservable(config.selected) {}

RadioButton::RadioButton(const std::string &elementName, const std::string &label, bool value)
    : ItemElement(elementName), Labellable(label), ValueObservable(value) {}

void RadioButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto oldValue = getValue();
  if (ImGui::RadioButton(getLabel().c_str(), getValue())) { setValueInner(true); }
  if (oldValue != getValue()) { notifyValueChanged(); }
}

bool RadioButton::isSelected() const { return getValue(); }

}  // namespace pf::ui::ig
