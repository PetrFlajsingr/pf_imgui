//
// Created by petr on 1/23/21.
//

#include "Tooltip.h"
#include <imgui.h>

namespace pf::ui::ig {

Tooltip::Tooltip(const std::string &elementName) : Element(elementName), Container(elementName) {}

void Tooltip::renderImpl() {
  ImGui::BeginTooltip();
  std::ranges::for_each(getChildren(), [&](auto &child) { child.render(); });
  ImGui::EndTooltip();
}

}// namespace pf::ui::ig