//
// Created by petr on 1/23/21.
//

#include "Tooltip.h"
#include <imgui.h>
#include <string>

namespace pf::ui::ig {

Tooltip::Tooltip(Tooltip::Config &&config) : ElementWithID(std::string{config.name.value}) {}

Tooltip::Tooltip(const std::string &elementName) : ElementWithID(elementName) {}

void Tooltip::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  ImGui::BeginTooltip();
  RAII end{ImGui::EndTooltip};
  std::ranges::for_each(getChildren(), &Renderable::render);
}

}  // namespace pf::ui::ig
