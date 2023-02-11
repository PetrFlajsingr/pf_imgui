//
// Created by petr on 10/31/20.
//

#include "Expander.h"
#include <imgui.h>

namespace pf::ui::ig {

Expander::Expander(Expander::Config &&config) : Expander(config.name, config.label, config.persistent ? Persistent::Yes : Persistent::No) {}

Expander::Expander(std::string_view elementName, std::string_view labelText, Persistent persistent)
    : ItemElement(elementName), Savable(persistent), label(std::string{labelText}), collapsed(false) {}

void Expander::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGui::SetNextItemOpen(!*collapsed);
  const auto flags = ImGuiTreeNodeFlags_DefaultOpen;
  *collapsed.modify() = !ImGui::CollapsingHeader(label->get().c_str(), flags);
  if (!*collapsed) { std::ranges::for_each(getChildren(), &Renderable::render); }
}

toml::table Expander::toToml() const { return toml::table({{"collapsed", *collapsed}}); }

void Expander::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("collapsed"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { *collapsed.modify() = *newVal; }
  }
}

}  // namespace pf::ui::ig
