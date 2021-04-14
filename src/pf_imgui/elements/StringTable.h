//
// Created by petr on 4/11/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_STRINGTABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_STRINGTABLE_H

#include <imgui.h>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Resizable.h>
#include <ranges>
#include <span>

namespace pf::ui::ig {

template<std::size_t ColumnCount>
using StringTableRow = std::array<std::string, ColumnCount>;

template<std::size_t ColumnCount>
struct TableSettings {
  std::optional<StringTableRow<ColumnCount>> header = std::nullopt;
  TableBorder border = TableBorder::None;
  bool resizableCols = false;
  bool reorderable = false;
  bool sortable = false;
  bool hideableCols = false;
  ImVec2 size = ImVec2{0, 0};
};

template<std::size_t ColumnCount>
class PF_IMGUI_EXPORT StringTable : public ItemElement, public Resizable {
 public:
  StringTable(const std::string &elementName, TableSettings<ColumnCount> &&settings)
      : ItemElement(elementName), Resizable(settings.size), header(settings.header),
        tableFlags(createFlags(settings.border, settings.resizableCols, settings.reorderable, settings.sortable,
                               settings.hideableCols)) {}

  void addRow(const std::ranges::range auto &vals) {
    assert(std::ranges::size(vals) == ColumnCount);
    std::ranges::copy(vals, std::back_inserter(rows));
  }
  void addRow(const ToStringConvertible auto &...vals) {
    static_assert(sizeof...(vals) == ColumnCount);
    (rows.template emplace_back(toString(vals)), ...);
  }
  void removeRow(std::size_t index) {
    const auto startIndex = index * ColumnCount;
    if (startIndex >= rows.size()) { return; }
    rows.erase(rows.begin() + startIndex, rows.begin() + startIndex + ColumnCount);
  }
  [[nodiscard]] std::span<std::string> getRow(std::size_t index) {
    const auto startIndex = index * ColumnCount;
    if (startIndex >= rows.size()) { return {}; }
    return {rows.begin() + startIndex, rows.begin() + startIndex + ColumnCount};
  }
  [[nodiscard]] std::string &getCell(std::size_t rowIdx, std::size_t columnIdx) {
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