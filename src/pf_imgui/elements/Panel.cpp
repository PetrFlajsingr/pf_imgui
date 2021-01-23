//
// Created by petr on 10/31/20.
//

#include "Panel.h"
#include <algorithm>
#include <imgui.h>
#include <range/v3/view/enumerate.hpp>
#include <utility>

namespace pf::ui::ig {

Panel::Panel(const std::string &elementName, std::string title, PanelLayout layout, const ImVec2 &panelSize)
    : Element(elementName), Resizable(panelSize), title(std::move(title)), panelLayout(layout) {}

void Panel::renderImpl() {
  ImGui::BeginChild(title.c_str(), getSize());
  if (panelLayout == PanelLayout::Vertical) {
    std::ranges::for_each(getChildren(), [&](auto &child) { child.render(); });
  } else {
    auto children = getChildren();
    if (!children.empty()) {
      const auto size = children.size();
      auto idx = 0u;
      std::ranges::for_each(children, [&](auto &child) {
        child.render();
        if (idx != size - 1) { ImGui::SameLine(); }
        ++idx;
      });
    }
  }
  ImGui::EndChild();
}

}// namespace pf::ui::ig