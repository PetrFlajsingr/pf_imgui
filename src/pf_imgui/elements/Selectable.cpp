//
// Created by petr on 5/22/21.
//

#include "Selectable.h"
#include <imgui.h>
#include <string>

namespace pf::ui::ig {

Selectable::Selectable(Selectable::Config &&config)
    : ItemElement(std::string{config.name.value}), ValueObservable(config.selected), size(config.size),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), label(std::string{config.label.value}) {}

Selectable::Selectable(const std::string &elementName, const std::string &labelText, bool initialValue, Size s,
                       Persistent persistent)
    : ItemElement(elementName), ValueObservable(initialValue), size(s), Savable(persistent), label(labelText) {}

toml::table Selectable::toToml() const { return toml::table{{"selected", getValue()}}; }

void Selectable::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("selected"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setValueAndNotifyIfChanged(*newVal); }
  }
}

void Selectable::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::Selectable(label->get().c_str(), getValueAddress(), 0, static_cast<ImVec2>(*size))) {
    notifyValueChanged();
  }
}

}  // namespace pf::ui::ig
