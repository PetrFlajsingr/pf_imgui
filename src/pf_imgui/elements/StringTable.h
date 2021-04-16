//
// Created by petr on 4/11/21.
//

#ifndef PF_IMGUI_ELEMENTS_STRINGTABLE_H
#define PF_IMGUI_ELEMENTS_STRINGTABLE_H

#include <imgui.h>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Resizable.h>
#include <ranges>
#include <span>

namespace pf::ui::ig {

/**
 * @brief Statically sized row for StringTable.
 */
template<std::size_t ColumnCount>
using StringTableRow = std::array<std::string, ColumnCount>;

/**
 * @brief Settings for table creation.
 * @tparam ColumnCount static column count of table
 */
template<std::size_t ColumnCount>
struct TableSettings {
  std::optional<StringTableRow<ColumnCount>> header = std::nullopt; /**< Optional header for table. If the header is not provided now header row is present in the table. */
  TableBorder border = TableBorder::None; /**< Type of border rendering. */
  bool resizableCols = false; /**< Make columns resizable. */
  bool reorderable = false; /**< Make columns reorderable. */
  bool sortable = false; /**< Make rows reorderable via clicking on header. */
  bool hideableCols = false; /**< Allow hiding of columns. */
  ImVec2 size = ImVec2{0, 0}; /**< Size of the table. */
};

/**
 * @brief Table using strings as cell values.
 * @tparam ColumnCount static column count
 */
template<std::size_t ColumnCount>
class PF_IMGUI_EXPORT StringTable : public ItemElement, public Resizable {
 public:
  /**
   * Construct StringTable.
   * @param elementName ID of the table
   * @param settings table settings
   */
  StringTable(const std::string &elementName, TableSettings<ColumnCount> &&settings)
      : ItemElement(elementName), Resizable(settings.size), header(settings.header),
        tableFlags(createFlags(settings.border, settings.resizableCols, settings.reorderable, settings.sortable,
                               settings.hideableCols)) {}

  /**
   * Add row to the tables end.
   * size(vals) must be equal to ColumnCount, otherwise undefined behavior.
   * @param vals values of cells in the row
   */
  void addRow(const std::ranges::range auto &vals) requires(ToStringConvertible<std::ranges::range_value_t<decltype(vals)>>) {
    assert(std::ranges::size(vals) == ColumnCount);
    std::ranges::copy(vals, std::back_inserter(rows));
  }
  /**
   * Add row to the tables end.
   * sizeof...(vals) must be equal to ColumnCount.
   * @param vals values of cells in the ro
   */
  void addRow(const ToStringConvertible auto &...vals) requires(sizeof...(vals) == ColumnCount) {
    (rows.template emplace_back(toString(vals)), ...);
  }
  /**
   * Remove row at provided index.
   * If the index is out of bounds nothing happens.
   * @param index index of the row to be deleted
   */
  void removeRow(std::size_t index) {
    const auto startIndex = index * ColumnCount;
    if (startIndex >= rows.size()) { return; }
    rows.erase(rows.begin() + startIndex, rows.begin() + startIndex + ColumnCount);
  }
  /**
   * Get view to the row at index.
   * If the index is out of bound an empty row is returned.
   * @param index index of the desired row
   * @return view to the row at index
   */
  [[nodiscard]] std::span<std::string> getRow(std::size_t index) {
    const auto startIndex = index * ColumnCount;
    if (startIndex >= rows.size()) { return {}; }
    return {rows.begin() + startIndex, rows.begin() + startIndex + ColumnCount};
  }
  /**
   * Get cell value at coordinates.
   * @param rowIdx index of a row
   * @param columnIdx index of a cell within the row
   * @return reference to the value at coordinates
   */
  [[nodiscard]] std::string &getCell(std::size_t rowIdx, std::size_t columnIdx) {
    auto row = getRow(rowIdx);
    assert(row.size() < columnIdx);
    return row[columnIdx];
  }
  /**
   * Get cell value at coordinates.
   * @param rowIdx index of a row
   * @param columnIdx index of a cell within the row
   * @return const reference to the value at coordinates
   */
  [[nodiscard]] const std::string &getCell(std::size_t rowIdx, std::size_t columnIdx) const {
    auto row = getRow(rowIdx);
    assert(row.size() < columnIdx);
    return row[columnIdx];
  }

 protected:
  void renderImpl() override {
    if (ImGui::BeginTable(getName().c_str(), ColumnCount, tableFlags, getSize())) {
      if (header.has_value()) {
        std::ranges::for_each(*header, [](const auto &value) { ImGui::TableSetupColumn(value.c_str()); });
        ImGui::TableHeadersRow();
      }

      std::ranges::for_each(rows | ranges::views::enumerate, [](const auto &record) {
        const auto &[idx, value] = record;
        if (idx % ColumnCount == 0) { ImGui::TableNextRow(); }
        ImGui::TableNextColumn();
        ImGui::Text(value.c_str());
      });
      ImGui::EndTable();
    }
  }

 private:
  std::optional<StringTableRow<ColumnCount>> header;
  std::vector<std::string> rows;
  ImGuiTableFlags tableFlags;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_STRINGTABLE_H
