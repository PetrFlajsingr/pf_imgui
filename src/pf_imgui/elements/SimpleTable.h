//
// Created by petr on 4/11/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SIMPLETABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SIMPLETABLE_H

#include <imgui.h>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <ranges>
#include <span>

namespace pf::ui::ig {

template<std::size_t ColumnCount>
class PF_IMGUI_EXPORT SimpleTable : public ItemElement {
 public:
  using Row = std::array<std::string, ColumnCount>;

  enum class Border : uint16_t {
    None = 0b0,
    HorizontalInner = 0b1,
    VerticalInner = 0b10,
    Inner = 0b11,
    HorizontalOuter = 0b100,
    VerticalOuter = 0b1000,
    Outer = 0b1100,
    Full = 0b1111
  };
  struct Settings {
    std::optional<Row> header = std::nullopt;
    Border border = Border::None;
    bool resizableCols = false;
    bool reorderable = false;
    bool sortable = false;
    bool hideableCols = false;
  };

  explicit SimpleTable(const std::string &elementName, Settings &&settings)
      : ItemElement(elementName), header(settings.header), tableFlags(createFlags(settings)) {}

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
    if (ImGui::BeginTable(getName().c_str(), ColumnCount, tableFlags)) {
      if (header.has_value()) {
        std::ranges::for_each(header, [](const auto &value) { ImGui::TableSetupColumn(value.c_str()); });
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
  static bool is(Border lhs, Border rhs) {
    using UnderType = std::underlying_type<Border>;
    return static_cast<UnderType>(lhs) & static_cast<UnderType>(rhs);
  }

  static ImGuiTableFlags createFlags(Settings &&settings) {
    auto result = ImGuiTableFlags{};
    if (is(settings.border, Border::HorizontalInner)) { result |= ImGuiTableFlags_BordersInnerH; }
    if (is(settings.border, Border::VerticalInner)) { result |= ImGuiTableFlags_BordersInnerV; }
    if (is(settings.border, Border::HorizontalOuter)) { result |= ImGuiTableFlags_BordersOuterH; }
    if (is(settings.border, Border::VerticalOuter)) { result |= ImGuiTableFlags_BordersOuterV; }
    if (settings.resizableCols) { result |= ImGuiTableFlags_Resizable; }
    if (settings.reorderable) { result |= ImGuiTableFlags_Reorderable; }
    if (settings.sortable) { result |= ImGuiTableFlags_Sortable; }
    if (settings.hideableCols) { result |= ImGuiTableFlags_Hideable; }
    return result;
  }
  std::optional<Row> header;
  std::vector<std::string> rows;
  ImGuiTableFlags_ tableFlags;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SIMPLETABLE_H
