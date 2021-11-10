//
// Created by petr on 1/24/21.
//

#include "GridLayout.h"
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

GridLayout::GridLayout(const std::string &elementName, const Size &size, uint32_t width, uint32_t height,
                       AllowCollapse allowCollapse, ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent), width(width), height(height) {
  const auto cellCount = width * height;
  cells.resize(cellCount);
  std::ranges::fill(cells, nullptr);
}

GridLayout::GridLayout(const std::string &elementName, const Size &size, uint32_t width, uint32_t height,
                       ShowBorder showBorder, Persistent persistent)
    : GridLayout(elementName, size, width, height, AllowCollapse::No, showBorder, persistent) {}

GridLayout::GridLayout(const std::string &elementName, const Size &size, uint32_t width, uint32_t height,
                       AllowCollapse allowCollapse, Persistent persistent)
    : GridLayout(elementName, size, width, height, allowCollapse, ShowBorder::No, persistent) {}

void GridLayout::setLayoutForCell(uint32_t column, uint32_t row, std::unique_ptr<ResizableLayout> layout) {
  cells[indexForCell(column, row)] = std::move(layout);
}
void GridLayout::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{[&] { ImGui::EndChild(); }};
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      const auto xCellSize = getSize().width.value / static_cast<float>(width);
      const auto yCellSize = getSize().height.value / static_cast<float>(height);
      const auto cellSize = Size{static_cast<uint32_t>(xCellSize), static_cast<uint32_t>(yCellSize)};
      for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
          const auto index = indexForCell(x, y);
          if (cells[index] != nullptr) {
            cells[index]->setSize(cellSize);
            ImGui::SetCursorPos(ImVec2{xCellSize * static_cast<float>(x), yCellSize * static_cast<float>(y)});
            cells[index]->render();
          }
        }
      }
    }
  }
}
uint32_t GridLayout::indexForCell(uint32_t column, uint32_t row) const { return row * width + column; }
ResizableLayout &GridLayout::getCellLayout(uint32_t column, uint32_t row) {
  const auto index = indexForCell(column, row);
#ifndef _MSC_VER// TODO: MSVC internal error
  if (index >= cells.size()) { throw InvalidArgumentException("Indices out of bounds: {}x{}", column, row); }
#endif
  return *cells[index];
}
std::vector<Renderable *> GridLayout::getRenderables() {
  return cells | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); }) | ranges::to_vector;
}
bool GridLayout::hasLayoutAt(uint32_t column, uint32_t row) {
  const auto index = indexForCell(column, row);
#ifndef _MSC_VER// TODO: MSVC internal error
  if (index >= cells.size()) { throw InvalidArgumentException("Indices out of bounds: {}x{}", column, row); }
#endif
  return cells[index] != nullptr;
}

}// namespace pf::ui::ig