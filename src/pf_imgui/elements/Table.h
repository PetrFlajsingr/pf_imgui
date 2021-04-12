//
// Created by petr on 4/12/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TABLE_H

#include <imgui.h>
#include <pf_common/tuple.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/StringTable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {

template<std::derived_from<Renderable>... Cells>
class PF_IMGUI_EXPORT Table : public ItemElement, public Labellable, public Resizable {
 public:
  using Row = std::tuple<std::unique_ptr<Cells>...>;
  constexpr static auto ColumnCount = sizeof...(Cells);

  Table(const std::string &elementName, TableSettings<ColumnCount> &&settings,
        const std::optional<std::string> &label = std::nullopt)
      : ItemElement(elementName), Labellable(label.template value_or("")), Resizable(settings.size),
        header(settings.header), tableFlags(createFlags(settings.border, settings.resizableCols, settings.reorderable,
                                                        settings.sortable, settings.hideableCols)) {
    Labellable::setLabelVisible(label.has_value() ? Visibility::Visible : Visibility::Invisible);
  }

  template<std::size_t TupleIndex, typename CurrentCell, typename... FollowingCells>
  class RowBuilder {
   public:
    explicit RowBuilder(Table &parent) requires(sizeof...(FollowingCells) == ColumnCount - 1) : table(parent) {}

    RowBuilder(Table &parent, Row &&row) : table(parent), resultRow(std::move(row)) {}

    template<typename... Args>
    auto operator()(Args &&...args) requires(std::constructible_from<CurrentCell, Args...>) {
      std::get<TupleIndex>(resultRow) = std::make_unique<CurrentCell>(std::forward<Args...>(args)...);
      return RowBuilder<TupleIndex + 1, FollowingCells...>{std::move(resultRow)};
    }

    void build() requires(sizeof...(FollowingCells) == 0) { table.rows.emplace_back(std::move(resultRow)); }

   private : Table &table;
    Row resultRow;
  };

  auto rowBuilder() { return RowBuilder<0, Cells...>{}; }

  void addRow(Row &&row) { rows.template emplace_back(std::move(row)); }

 protected:
  void renderImpl() override {
    if (ImGui::BeginTable(getName().c_str(), ColumnCount, tableFlags, getSize())) {
      if (header.has_value()) {
        std::ranges::for_each(*header, [](const auto &value) { ImGui::TableSetupColumn(value.c_str()); });
        ImGui::TableHeadersRow();
      }

      std::ranges::for_each(rows, [](const auto &row) {
        ImGui::TableNextRow();
        iterateTuple(
            [](Renderable &column) {
              ImGui::TableNextColumn();
              column.render();
            },
            row);
      });
      ImGui::EndTable();
    }
  }

 private:
  std::optional<StringTableRow<ColumnCount>> header;
  std::vector<Row> rows;
  ImGuiTableFlags tableFlags;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TABLE_H
