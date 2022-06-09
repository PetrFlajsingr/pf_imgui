//
// Created by petr on 1/23/21.
//

#include "Tooltip.h"
#include <imgui.h>
#include <string>

namespace pf::ui::ig {

Tooltip::Tooltip(Tooltip::Config &&config) : Element(std::string{config.name.value}) {}

Tooltip::Tooltip(const std::string &elementName) : Element(elementName) {}

void Tooltip::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  ImGui::BeginTooltip();
  RAII end{ImGui::EndTooltip};
  std::ranges::for_each(getChildren(), &Renderable::render);
}

}  // namespace pf::ui::ig
