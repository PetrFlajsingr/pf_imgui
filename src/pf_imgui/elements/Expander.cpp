//
// Created by petr on 10/31/20.
//

#include "Expander.h"
#include <imgui.h>

namespace pf::ui::ig {

Expander::Expander(Expander::Config &&config)
    : ItemElement(std::string{config.name.value}),
      Collapsible(config.allowCollapse ? AllowCollapse::Yes : AllowCollapse::No,
                  config.persistent ? Persistent::Yes : Persistent::No),
      label(std::string{config.label.value}) {}

Expander::Expander(const std::string &elementName, const std::string &labelText, Persistent persistent,
                   AllowCollapse allowCollapse)
    : ItemElement(elementName), Collapsible(allowCollapse, persistent), label(labelText) {}

Expander::Expander(const std::string &elementName, const std::string &labelText, AllowCollapse allowCollapse)
    : Expander(elementName, labelText, Persistent::No, allowCollapse) {}

void Expander::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto shouldBeOpen = !isCollapsed() || !isCollapsible();
  ImGui::SetNextItemOpen(shouldBeOpen);
  const auto flags = ImGuiTreeNodeFlags_DefaultOpen;
  setCollapsed(!ImGui::CollapsingHeader(label->get().c_str(), flags));
  if (!isCollapsed()) { std::ranges::for_each(getChildren(), &Renderable::render); }
}

}  // namespace pf::ui::ig
