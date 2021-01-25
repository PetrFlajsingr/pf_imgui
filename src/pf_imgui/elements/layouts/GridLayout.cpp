//
// Created by petr on 1/24/21.
//

#include "GridLayout.h"
#include "AbsoluteLayout.h"

namespace pf::ui::ig {
GridLayout::GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height,
                       bool showBorder)
    : Layout(elementName, size, showBorder), width(width), height(height) {}

void GridLayout::renderImpl() {
  if (ImGui::BeginChild(getName().c_str(), getSize(), isDrawBorder())) {
    const auto xCellSize = getSize().x / width;
    const auto yCellSize = getSize().y / height;
    for (uint32_t y = 0; y < height; ++y) {
      for (uint32_t x = 0; x < width; ++x) {
        if (ImGui::BeginChild((getName() + std::to_string(y) + std::to_string(x)).c_str(),
                              ImVec2{xCellSize * x, yCellSize * y}, true)) {}
        ImGui::EndChild();
      }
    }
  }
  ImGui::EndChild();
}

}// namespace pf::ui::ig