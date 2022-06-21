//
// Created by petr.flajsingr on 2/9/2022.
//

#include "Toggle.h"
#include <pf_imgui/details/ToggleButton.h>

namespace pf::ui::ig {

Toggle::Toggle(Toggle::Config &&config)
    : ItemElement(std::string{config.name.value}), ValueObservable(config.enabled),
      Labellable(std::string{config.label.value}), Savable(config.persistent ? Persistent::Yes : Persistent::No) {}

Toggle::Toggle(const std::string &elementName, const std::string &label, bool value, Persistent persistent)
    : ItemElement(elementName), ValueObservable(value), Labellable(label), Savable(persistent) {}

void Toggle::setSelected(bool selected) { setValueAndNotifyIfChanged(selected); }

bool Toggle::isSelected() const { return getValue(); }

void Toggle::toggle() { setSelected(!isSelected()); }

toml::table Toggle::toToml() const { return toml::table{{"checked", getValue()}}; }

void Toggle::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("checked"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setValueAndNotifyIfChanged(*newVal); }
  }
}

void Toggle::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ToggleButton(getLabel().c_str(), getValueAddress())) { notifyValueChanged(); }
}

}  // namespace pf::ui::ig
