//
// Created by petr on 10/31/20.
//

#include "RadioButton.h"
#include <imgui.h>

namespace pf::ui::ig {

RadioButton::RadioButton(RadioButton::Config &&config)
    : ItemElement(std::string{config.name.value}), selected(config.selected), label(std::string{config.label.value}) {}

RadioButton::RadioButton(const std::string &elementName, const std::string &labelText, bool initialValue)
    : ItemElement(elementName), selected(initialValue), label(labelText) {}

void RadioButton::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::RadioButton(label->get().c_str(), getValue())) { *selected.modify() = true; }
}

const bool &RadioButton::getValue() const { return *selected; }

void RadioButton::setValue(const bool &newValue) { *selected.modify() = newValue; }

Subscription RadioButton::addValueListenerImpl(std::function<void(const bool &)> listener) {
  return selected.addListener(std::move(listener));
}

}  // namespace pf::ui::ig
