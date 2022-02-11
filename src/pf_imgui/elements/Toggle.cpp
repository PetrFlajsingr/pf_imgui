//
// Created by petr.flajsingr on 2/9/2022.
//

#include "Toggle.h"
#include <pf_imgui/details/ToggleButton.h>

namespace pf::ui::ig {

Toggle::Toggle(Toggle::Config &&config)
    : ItemElement(std::string{config.name}), ValueObservable(config.enabled), Labellable(std::string{config.label}),
      Savable(config.persistent) {}

Toggle::Toggle(const std::string &elementName, const std::string &label, bool value, Persistent persistent)
    : ItemElement(elementName), ValueObservable(value), Labellable(label), Savable(persistent) {}

void Toggle::setSelected(bool selected) { setValueAndNotifyIfChanged(selected); }

bool Toggle::isSelected() const { return getValue(); }

void Toggle::toggle() { setSelected(!isSelected()); }

void Toggle::unserialize_impl(const toml::table &src) {
  if (auto newValIter = src.find("checked"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setValueAndNotifyIfChanged(*newVal); }
  }
}

toml::table Toggle::serialize_impl() const { return toml::table{{"checked", getValue()}}; }

void Toggle::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (ToggleButton(getLabel().c_str(), getValueAddress())) { notifyValueChanged(); }
}

}  // namespace pf::ui::ig