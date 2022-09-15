//
// Created by petr.flajsingr on 2/9/2022.
//

#include "Toggle.h"
#include <pf_imgui/details/ToggleButton.h>

namespace pf::ui::ig {

Toggle::Toggle(Toggle::Config &&config)
    : Toggle(config.name, config.label, config.enabled, config.persistent ? Persistent::Yes : Persistent::No) {}

Toggle::Toggle(std::string_view elementName, std::string_view labelText, bool initialValue, Persistent persistent)
    : ItemElement(elementName), Savable(persistent), label(std::string{labelText}), selected(initialValue) {}

void Toggle::toggle() { *selected.modify() = !*selected; }

toml::table Toggle::toToml() const { return toml::table{{"checked", getValue()}}; }

void Toggle::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("checked"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { *selected.modify() = *newVal; }
  }
}

void Toggle::setValue(const bool &newValue) { *selected.modify() = newValue; }

const bool &Toggle::getValue() const { return *selected; }

Subscription Toggle::addValueListenerImpl(std::function<void(const bool &)> listener) {
  return selected.addListener(std::move(listener));
}

void Toggle::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ToggleButton(label->get().c_str(), &selected.value)) { selected.triggerListeners(); }
}

}  // namespace pf::ui::ig
