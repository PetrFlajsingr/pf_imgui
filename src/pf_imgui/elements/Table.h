/**
* @file Table.h
* @brief Table element.
* @author Petr Flajšingr
* @date 12.4.21
*/

#ifndef PF_IMGUI_ELEMENTS_TABLE_H
#define PF_IMGUI_ELEMENTS_TABLE_H

#include <imgui.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/RenderablesContainer.h>
#include <pf_imgui/interface/Resizable.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

template<std::size_t ColumnCount>
struct TableSettings {
  std::optional<std::array<std::string, ColumnCount>> header = std::nullopt;
  Flags<TableBorder> border;   /**< Type of border rendering. */
  Flags<TableOptions> options; /**< Interaction options. */
  Size size = Size::Auto();    /**< Size of the table. */
};

/**
 * @brief A table containing any renderable element in each cell.
 *
 * @tparam ColumnCount static column count of the table
 */
template<std::size_t ColumnCount>
class Table : public Element, public RenderablesContainer, public Resizable {
  using Cells = std::array<std::unique_ptr<Renderable>, ColumnCount>;
  struct Row {
    const std::size_t id;
    Cells cells;
  };

 public:
  /**
   * Construct Table.
   * @param elementName unique name of the element
   * @param settings rendering and behavioral settings
   */
  Table(const std::string &elementName, const TableSettings<ColumnCount> &settings)
      : Element(elementName), Resizable(settings.size), header(settings.header),
        flags(CreateFlags(settings.border, settings.options)) {}

  std::vector<Renderable *> getRenderables() override {
    return rows | ranges::view::transform(&Row::cells) | ranges::view::join
        | ranges::view::transform(&std::unique_ptr<Renderable>::get) | ranges::to_vector;
  }

  /**
   * Add a new row of elements.
   * @param row elements to be added
   * @return unique identifier of the newly added row. It can be used to remove this row.
   */
  std::size_t addRow(Cells &&row) {
    const auto rowId = idCounter++;
    rows.emplace_back(idCounter++, std::move(row));
    return rowId;
  }

  /**
   * Remove row based on its unique id.
   * @param rowId id of the row to be removed
   */
  void removeRow(std::size_t rowId) {
    auto remove = std::ranges::remove(rows, rowId, &Row::id);
    rows.erase(remove.begin(), remove.end());
  }

  /**
   *
   * @param id unique row id
   * @return row with given id if found, otherwise nullopt
   */
  [[nodiscard]] std::optional<std::reference_wrapper<const Row>> getRowById(std::size_t id) {
    if (auto iter = std::ranges::find(rows, id, &Row::id); iter != rows.end()) { return *iter; }
    return std::nullopt;
  }
  /**
   *
   * @param index index of a row
   * @return row with given index if found, otherwise nullopt
   */
  [[nodiscard]] std::optional<std::reference_wrapper<const Row>> getRowByIndex(std::size_t index) {
    if (index < rows.size()) { return rows[index]; }
    return std::nullopt;
  }

  [[nodiscard]] std::size_t getColumnCount() const { return ColumnCount; }

  [[nodiscard]] std::size_t getRowCount() const { return rows.size(); }

 protected:
  void renderImpl() override {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    if (ImGui::BeginTable(getName().c_str(), ColumnCount, flags, getSize().asImVec())) {
      RAII end{[] { ImGui::EndTable(); }};
      if (header.has_value()) {
        std::ranges::for_each(*header, [](const auto &value) { ImGui::TableSetupColumn(value.c_str()); });
        ImGui::TableHeadersRow();
      }

      std::ranges::for_each(rows | ranges::views::transform(&Row::cells), [](auto &row) {
        ImGui::TableNextRow();
        std::ranges::for_each(row, [](auto &cell) {
          if (ImGui::TableNextColumn()) { cell->render(); }
        });
      });
    }
  }

 private:
  static ImGuiTableFlags CreateFlags(const Flags<TableBorder> &tableBorder, const Flags<TableOptions> &options) {
    return ImGuiTableFlags{*tableBorder | *options};
  }

  std::optional<std::array<std::string, ColumnCount>> header;
  ImGuiTableFlags flags;

  std::vector<Row> rows;

  std::size_t idCounter = 0;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_TABLE_H
