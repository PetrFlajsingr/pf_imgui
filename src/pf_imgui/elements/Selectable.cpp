//
// Created by petr on 5/22/21.
//

#include "Selectable.h"
#include <imgui.h>
#include <string>

namespace pf::ui::ig {

Selectable::Selectable(Selectable::Config &&config)
    : ItemElement(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable(config.selected),
      Resizable(config.size), Savable(config.persistent ? Persistent::Yes : Persistent::No) {}

Selectable::Selectable(const std::string &elementName, const std::string &label, bool value, Size s,
                       Persistent persistent)
    : ItemElement(elementName), Labellable(label), ValueObservable(value), Resizable(s), Savable(persistent) {}

toml::table Selectable::toToml() const { return toml::table{{"selected", getValue()}}; }

void Selectable::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("selected"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setValueAndNotifyIfChanged(*newVal); }
  }
}

void Selectable::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (ImGui::Selectable(getLabel().c_str(), getValueAddress(), 0, static_cast<ImVec2>(getSize()))) {
    notifyValueChanged();
  }
}

}  // namespace pf::ui::ig
