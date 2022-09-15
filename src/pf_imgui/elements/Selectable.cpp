//
// Created by petr on 5/22/21.
//

#include "Selectable.h"
#include <imgui.h>
#include <string>

namespace pf::ui::ig {

Selectable::Selectable(Selectable::Config &&config)
    : Selectable(config.name, config.label, config.selected, config.size,
                 config.persistent ? Persistent::Yes : Persistent::No) {}

Selectable::Selectable(std::string_view elementName, std::string_view labelText, bool initialValue, Size s,
                       Persistent persistent)
    : ItemElement(elementName), Savable(persistent), label(std::string{labelText}), size(s), selected(initialValue) {}

toml::table Selectable::toToml() const { return toml::table{{"selected", getValue()}}; }

void Selectable::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("selected"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { *selected.modify() = *newVal; }
  }
}

void Selectable::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::Selectable(label->get().c_str(), &selected.value, 0, static_cast<ImVec2>(*size))) {
    selected.triggerListeners();
  }
}
const bool &Selectable::getValue() const { return *selected; }

void Selectable::setValue(const bool &newValue) { *selected.modify() = newValue; }

Subscription Selectable::addValueListenerImpl(std::function<void(const bool &)> listener) {
  return selected.addListener(std::move(listener));
}

}  // namespace pf::ui::ig
