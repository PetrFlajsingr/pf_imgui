//
// Created by petr on 10/31/20.
//

#include "Checkbox.h"
#include <imgui.h>
#include <pf_imgui/details/ToggleButton.h>

namespace pf::ui::ig {

Checkbox::Checkbox(Checkbox::Config &&config)
    : ItemElement(std::string{config.name.value}), Savable(config.persistent ? Persistent::Yes : Persistent::No),
      label(std::string{config.label.value}), selected(config.selected) {}

Checkbox::Checkbox(std::string_view elementName, std::string_view labelText, bool initialValue, Persistent persistent)
    : ItemElement(elementName), Savable(persistent), label(std::string{labelText}), selected(initialValue) {}

void Checkbox::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::Checkbox(label->get().c_str(), &selected.value)) { selected.triggerListeners(); }
}

toml::table Checkbox::toToml() const { return toml::table{{"checked", getValue()}}; }

void Checkbox::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("checked"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { *selected.modify() = *newVal; }
  }
}

const bool &Checkbox::getValue() const { return *selected; }

void Checkbox::setValue(const bool &newValue) { *selected.modify() = newValue; }

Subscription Checkbox::addValueListenerImpl(std::function<void(const bool &)> listener) {
  return selected.addListener(std::move(listener));
}

}  // namespace pf::ui::ig
