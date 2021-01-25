//
// Created by petr on 1/24/21.
//

#include "GridLayout.h"
#include <pf_common/exceptions/StackTraceException.h>

namespace pf::ui::ig {

GridLayout::GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height,
                       bool showBorder)
    : Layout(elementName, size, showBorder), width(width), height(height) {
  const auto cellCount = width * height;
  cells.reserve(cellCount);
}

void GridLayout::setLayoutForCell(uint32_t column, uint32_t row, std::unique_ptr<Layout> layout) {
  cells[indexForCell(column, row)] = std::move(layout);
}

void GridLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSize(), isDrawBorder(), flags)) {
    const auto xCellSize = getSize().x / width;
    const auto yCellSize = getSize().y / height;
    const auto cellSize = ImVec2{xCellSize, yCellSize};
    for (uint32_t y = 0; y < height; ++y) {
      for (uint32_t x = 0; x < width; ++x) {
        const auto index = indexForCell(x, y);
        if (cells[index] != nullptr) {
          cells[index]->setSize(cellSize);
          ImGui::SetCursorPos(ImVec2{xCellSize * x, yCellSize * y});
          cells[index]->render();
        }
      }
    }
  }
  ImGui::EndChild();
}
uint32_t GridLayout::indexForCell(uint32_t column, uint32_t row) const { return row * width + column; }

Layout &GridLayout::getCellLayout(uint32_t column, uint32_t row) {
  const auto index = indexForCell(column, row);
  if (index >= cells.size()) { throw StackTraceException::fmt("Indices out of bounds: {}x{}", column, row); }
  return *cells[index];
}

}// namespace pf::ui::ig