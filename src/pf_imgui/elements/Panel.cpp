//
// Created by petr on 10/31/20.
//

#include "Panel.h"
#include "imgui.h"
#include <algorithm>
#include <utility>

namespace pf::ui::ig {

Panel::Panel(const std::string &elementName, std::string title, PanelLayout layout,
                       const ImVec2 &panelSize)
    : Element(elementName), Container(elementName), ResizableElement(elementName, panelSize), title(std::move(title)), panelLayout(layout) {}

void Panel::renderImpl() {
  ImGui::BeginChild(title.c_str(), getSize());
  if (panelLayout == PanelLayout::Vertical) {
    std::ranges::for_each(getChildren(), [&](auto &child) { child.get().render(); });
  } else {
    if (!getChildren().empty()) {
      std::ranges::for_each_n(getChildren().begin(), getChildren().size() - 1, [&](auto &child) {
        child.get().render();
        ImGui::SameLine();
      });
      getChildren().back().get().render();
    }
  }
  ImGui::EndChild();
}

}// namespace pf::ui