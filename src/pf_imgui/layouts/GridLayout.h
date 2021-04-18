/**
 * @file GridLayout.h
 * @brief Layout dividing its area into a grid.
 * @author Petr Flajšingr
 * @date 24.1.21
 */

#ifndef PF_IMGUI_LAYOUTS_GRIDLAYOUT_H
#define PF_IMGUI_LAYOUTS_GRIDLAYOUT_H

#include "ResizableLayout.h"
#include <algorithm>
#include <concepts>
#include <memory>
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/exceptions.h>
#include <vector>

namespace pf::ui::ig {
/**
 * @brief A layout which positions it's children in a grid.
 *
 * Only descendants of Layout interface are allowed as children, in order to avoid weird element positioning.
 */
class PF_IMGUI_EXPORT GridLayout : public ResizableLayout {
 public:
  /**
   * Construct GridLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param width width in cells - amount of cells in each row
   * @param height height in cells - amount of cells in each column
   * @param allowCollapse enable collapse button
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height,
             AllowCollapse allowCollapse = AllowCollapse::No, ShowBorder showBorder = ShowBorder::No,
             Persistent persistent = Persistent::No);
  /**
   * Construct GridLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param width width in cells - amount of cells in each row
   * @param height height in cells - amount of cells in each columnn
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height, ShowBorder showBorder,
             Persistent persistent = Persistent::No);
  /**
   * Construct GridLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param width width in cells - amount of cells in each row
   * @param height height in cells - amount of cells in each column
   * @param allowCollapse enable collapse button
   * @param persistent enable state saving
   */
  GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height,
             AllowCollapse allowCollapse, Persistent persistent = Persistent::No);

  /**
   * Set layout for a selected cell.
   * @param column column of the selected cell
   * @param row row of the selected cell
   * @param layout layout to be moved
   * @throws InvalidArgumentException when coordinates are out of bounds
   */
  void setLayoutForCell(uint32_t column, uint32_t row, std::unique_ptr<ResizableLayout> layout);

  /**
   * Create a layout on given coordinates. If a layout is already present there it is overwritten.
   * @tparam T type of the created Layout
   * @tparam Args constructor argument types for T
   * @param column column of the selected cell
   * @param row row of the selected cell
   * @param name ID of the newly created layout
   * @param args arguments passed to Ts constructor
   * @return reference to the newly created Layout
   * @throws InvalidArgumentException when coordinates are out of bounds
   * @throws DuplicateIdException when an ID is already present in the container
   */
  template<typename T, typename... Args>
  requires std::derived_from<T, Layout> && std::constructible_from<T, std::string, Args...> T &
  createLayout(uint32_t column, uint32_t row, std::string name, Args &&...args) {
    if (findIf(cells, [name](const auto &cell) { return cell->getName() == name; }).has_value()) {
      throw DuplicateIdException("{} already present in ui", name);
    }
    auto child = std::make_unique<T>(name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    const auto index = indexForCell(column, row);
    if (index >= cells.size()) { throw InvalidArgumentException("Indices out of bounds: {}x{}", column, row); }
    cells[index] = std::move(child);
    return *ptr;
  }

  /**
   * Get Layout stored at the given coordinates.
   * @param column column of the cell
   * @param row row of the cell
   * @return reference to layout at the given coordinates
   * @throws InvalidArgumentException when coordinates are out of bounds
   */
  [[nodiscard]] ResizableLayout &getCellLayout(uint32_t column, uint32_t row);

  /**
   * Check whether a cell contains a Layout or not.
   * @param column column of the cell
   * @param row row of the cell
   * @return true if the cell is filled, false otherwise
   * @throws InvalidArgumentException when coordinates are out of bounds
   */
  [[nodiscard]] bool hasLayoutAt(uint32_t column, uint32_t row);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  uint32_t width;
  uint32_t height;
  std::vector<std::unique_ptr<ResizableLayout>> cells;

  /**
   * Translate 2D coordinates to 1D for lookup in cell storage.
   * @param column column of the cell
   * @param row row of the cell
   * @return 1D index for cells
   */
  [[nodiscard]] uint32_t indexForCell(uint32_t column, uint32_t row) const;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_LAYOUTS_GRIDLAYOUT_H
