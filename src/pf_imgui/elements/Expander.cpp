//
// Created by petr on 10/31/20.
//

#include "Expander.h"
#include <imgui.h>

namespace pf::ui::ig {

Expander::Expander(Expander::Config &&config)
    : ItemElement(std::string{config.name}), Labellable(std::string{config.label}),
      Collapsible(config.allowCollapse, config.persistent) {}

Expander::Expander(const std::string &elementName, const std::string &label, Persistent persistent,
                   AllowCollapse allowCollapse)
    : ItemElement(elementName), Labellable(label), Collapsible(allowCollapse, persistent) {}

Expander::Expander(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse)
    : Expander(elementName, label, Persistent::No, allowCollapse) {}

void Expander::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  const auto shouldBeOpen = !isCollapsed() || !isCollapsible();
  ImGui::SetNextItemOpen(shouldBeOpen);
  const auto flags = ImGuiTreeNodeFlags_DefaultOpen;
  setCollapsed(!ImGui::CollapsingHeader(getLabel().c_str(), flags));
  if (!isCollapsed()) { std::ranges::for_each(getChildren(), &Renderable::render); }
}

}  // namespace pf::ui::ig
