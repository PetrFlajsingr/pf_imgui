//
// Created by petr on 5/22/21.
//

#include "Selectable.h"
#include <imgui.h>
#include <string>

namespace pf::ui::ig {

Selectable::Selectable(Selectable::Config &&config)
    : ItemElement(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable(config.selected),
      Resizable(config.size), Savable(config.persistent) {}

Selectable::Selectable(const std::string &elementName, const std::string &label, bool value, Size s,
                       Persistent persistent)
    : ItemElement(elementName), Labellable(label), ValueObservable(value), Resizable(s), Savable(persistent) {}

void Selectable::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (ImGui::Selectable(getLabel().c_str(), getValueAddress(), 0, getSize().asImVec())) { notifyValueChanged(); }
}

void Selectable::unserialize_impl(const toml::table &src) {
  if (auto newValIter = src.find("selected"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setValueAndNotifyIfChanged(*newVal); }
  }
}

toml::table Selectable::serialize_impl() const { return toml::table{{"selected", getValue()}}; }

}  // namespace pf::ui::ig
