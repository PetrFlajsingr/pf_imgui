//
// Created by petr on 1/23/21.
//

#include "Tooltip.h"
#include <imgui.h>
#include <string>

namespace pf::ui::ig {

Tooltip::Tooltip(Tooltip::Config &&config) : Tooltip(config.name) {}

Tooltip::Tooltip(std::string_view elementName) : ElementWithID(elementName) {}

void Tooltip::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGui::BeginTooltip();
  ScopeExit end{&ImGui::EndTooltip};
  std::ranges::for_each(getChildren(), &Renderable::render);
}

}  // namespace pf::ui::ig
