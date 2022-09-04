//
// Created by petr on 1/24/21.
//

#include "GridLayout.h"
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>

namespace pf::ui::ig {

GridLayout::GridLayout(GridLayout::Config &&config)
    : Layout(std::string{config.name.value}, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No),
      width(config.widthInCells), height(config.heightInCells) {
  const auto cellCount = width * height;
  cells.resize(cellCount);
  std::ranges::fill(cells, nullptr);
}

GridLayout::GridLayout(const std::string &elementName, const Size &initialSize, uint32_t gridWidth, uint32_t gridHeight,
                       ShowBorder showBorder)
    : Layout(elementName, initialSize, showBorder), width(gridWidth), height(gridHeight) {
  const auto cellCount = width * height;
  cells.resize(cellCount);
  std::ranges::fill(cells, nullptr);
}

void GridLayout::setLayoutForCell(uint32_t column, uint32_t row, std::unique_ptr<Layout> layout) {
  cells[indexForCell(column, row)] = std::move(layout);
}

void GridLayout::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(*size), isDrawBorder(), flags)) {
    auto scrollApplier = applyScroll();
    const auto xCellSize = static_cast<float>(size->width) / static_cast<float>(width);
    const auto yCellSize = static_cast<float>(size->height) / static_cast<float>(height);
    const auto cellSize = Size{xCellSize, yCellSize};
    for (uint32_t y = 0; y < height; ++y) {
      for (uint32_t x = 0; x < width; ++x) {
        const auto index = indexForCell(x, y);
        if (cells[index] != nullptr) {
          *cells[index]->size.modify() = cellSize;
          ImGui::SetCursorPos(ImVec2{xCellSize * static_cast<float>(x), yCellSize * static_cast<float>(y)});
          cells[index]->render();
        }
      }
    }
  }
}

uint32_t GridLayout::indexForCell(uint32_t column, uint32_t row) const { return row * width + column; }

Layout &GridLayout::getCellLayout(uint32_t column, uint32_t row) {
  const auto index = indexForCell(column, row);
  return *cells[index];
}

std::vector<Renderable *> GridLayout::getRenderables() {
  return cells | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); }) | ranges::to_vector;
}

bool GridLayout::hasLayoutAt(uint32_t column, uint32_t row) {
  const auto index = indexForCell(column, row);
  return cells[index] != nullptr;
}

std::uint32_t GridLayout::getWidth() const { return width; }

std::uint32_t GridLayout::getHeight() const { return height; }

}  // namespace pf::ui::ig
