//
// Created by petr on 1/24/21.
//

#include "GridLayout.h"
#include <pf_common/exceptions/StackTraceException.h>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

GridLayout::GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height,
                       AllowCollapse allowCollapse, ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent), width(width), height(height) {
  const auto cellCount = width * height;
  cells.resize(cellCount);
  std::ranges::fill(cells, nullptr);
}

GridLayout::GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height,
                       ShowBorder showBorder, Persistent persistent)
    : GridLayout(elementName, size, width, height, AllowCollapse::No, showBorder, persistent) {}

GridLayout::GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height,
                       AllowCollapse allowCollapse, Persistent persistent)
    : GridLayout(elementName, size, width, height, allowCollapse, ShowBorder::No, persistent) {}

void GridLayout::setLayoutForCell(uint32_t column, uint32_t row, std::unique_ptr<ResizableLayout> layout) {
  cells[indexForCell(column, row)] = std::move(layout);
}
void GridLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
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
  }
  ImGui::EndChild();
}
uint32_t GridLayout::indexForCell(uint32_t column, uint32_t row) const { return row * width + column; }
ResizableLayout &GridLayout::getCellLayout(uint32_t column, uint32_t row) {
  const auto index = indexForCell(column, row);
  if (index >= cells.size()) { throw StackTraceException::fmt("Indices out of bounds: {}x{}", column, row); }
  return *cells[index];
}
std::vector<Renderable *> GridLayout::getRenderables() {
  return cells | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); }) | ranges::to_vector;
}

}// namespace pf::ui::ig