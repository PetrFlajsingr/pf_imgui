//
// Created by petr on 4/12/21.
//

#ifndef PF_IMGUI_ELEMENTS_TABLE_H
#define PF_IMGUI_ELEMENTS_TABLE_H

#include <imgui.h>
#include <pf_common/Visitor.h>
#include <pf_common/tuple.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/StringTable.h>
#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/meta.h>

namespace pf::ui::ig {
template<typename Factory, std::derived_from<Renderable>... Cells>
class TableWithFactory;
/**
 * Convenience alias for table without factory.
 * @tparam Cells types of elements within cells on each row
 */
template<std::derived_from<Renderable>... Cells>
using Table = TableWithFactory<CommonPlaceholder, Cells...>;
/**
 * Statically sized row of TableWithFactory.
 */
template<std::derived_from<Renderable>... Cells>
using TableRow = std::tuple<std::size_t, std::unique_ptr<Cells>...>;

/**
 * @brief Requirements for RowFactory.
 * A row factory is a utility object which allows for simplified addition of rows.
 * @tparam T Factory type
 * @tparam Cells element types for cells within table rows
 */
template<typename T, typename... Cells>
concept RowFactory = requires {
  typename T::CreateType; /**< Type from which the row should be created. */
}
&&requires(T t, typename T::CreateType source) {
  { t.create(source) } -> std::same_as<TableRow<Cells...>>; /**< Creation of new row */
};

/**
 * @brief Table, which can contain any Renderable as a cell value.
 *
 * Table width is statically sized. Each column should contain the same type of element, thus Cells... are used.
 *
 * @tparam Factory factory for utility method addRow(Factory::CreationType), if factory is CommonPlaceholder, then no factory is used
 * @tparam Cells types of Renderables used for each column
 *
 * @todo: row insert, remove
 */
template<typename Factory, std::derived_from<Renderable>... Cells>
class PF_IMGUI_EXPORT TableWithFactory : public ItemElement, public Labellable, public Resizable {
 public:
  constexpr static auto HasFactory = RowFactory<Factory, Cells...>;
  static_assert(HasFactory || std::same_as<Factory, CommonPlaceholder>);
  using Id = std::size_t; /**< row id */
  using Row = TableRow<Cells...>; /**< row storage type */
  constexpr static auto ColumnCount = sizeof...(Cells);

  /**
   * Construct TableWithFactory.
   * @param elementName ID of the table
   * @param settings settings of the table
   * @param label text rendered above the table
   */
  TableWithFactory(const std::string &elementName, TableSettings<ColumnCount> &&settings,
                   const std::optional<std::string> &label =
                       std::nullopt) requires(!HasFactory || std::is_default_constructible_v<Factory>)
      : ItemElement(elementName), Labellable(label.template value_or("")), Resizable(settings.size),
        header(settings.header), tableFlags(createFlags(settings.border, settings.resizableCols, settings.reorderable,
                                                        settings.sortable, settings.hideableCols)) {
    Labellable::setLabelVisible(label.has_value() ? Visibility::Visible : Visibility::Invisible);
  }

  /**
   * Construct TableWithFactory.
   * @param elementName ID of the table
   * @param settings settings of the table
   * @param factory instance of Factory for custom row creation
   * @param label text rendered above the table
   */
  TableWithFactory(const std::string &elementName, TableSettings<ColumnCount> &&settings, Factory &&factory,
                   const std::optional<std::string> &label = std::nullopt) requires(HasFactory)
      : ItemElement(elementName), Labellable(label.template value_or("")), Resizable(settings.size),
        header(settings.header), tableFlags(createFlags(settings.border, settings.resizableCols, settings.reorderable,
                                                        settings.sortable, settings.hideableCols)),
        factory(std::forward<Factory>(factory)) {
    Labellable::setLabelVisible(label.has_value() ? Visibility::Visible : Visibility::Invisible);
  }
  /**
   * @brief Utility for semi simple creation of rows.
   * @tparam WasClickable should be true if the previous type was clickable, in order to allow addClickListener method
   * @tparam WasObservable should be true if the previous type was ValueObservable, in order to allow addValueListener method
   * @tparam TupleIndex index for std::get
   * @tparam CurrentCell first type in type list
   * @tparam FollowingCells following types in type list
   */
  template<bool WasClickable, bool WasObservable, std::size_t TupleIndex, typename CurrentCell,
           typename... FollowingCells>
  class RowBuilder;
  /**
   * @brief Special case of RowBuilder, which handles last element in a tuple.
   * @tparam WasClickable should be true if the previous type was clickable, in order to allow addClickListener method
   * @tparam WasObservable should be true if the previous type was ValueObservable, in order to allow addValueListener method
   * @tparam TupleIndex index for std::get
   */
  template<bool WasClickable, bool WasObservable, std::size_t TupleIndex>
  class RowBuilderFinish;

  /**
   * Create row using a builder.
   * Usage: table.rowBuilder()("button1").addClickListener([]{})("text1")("combobox1").build()
   * @return builder for row
   */
  auto rowBuilder() { return RowBuilder<false, false, 0, Cells...>{*this}; }

  /**
   * Add pre-built row.
   * @param row row to be added
   */
  void addRow(Row &&row) { rows.template emplace_back(std::move(row)); }

  /**
   * Add a new row using Factory to create it.
   * @param source object which serves as a source for rows creation
   * @return reference to the newly created row
   */
  Row &addRow(typename Factory::CreateType &&source) requires(HasFactory) {
    auto row = factory.create(std::forward<typename Factory::CreateType>(source));
    addRow(std::move(row));
    return rows.back();
  }

  /**
   * Get row with the given ID.
   * @param ID index of the desired row
   * @return reference to row
   * @throws InvalidArgumentException when no row with such ID exists
   */
  auto getRow(Id id) {
    auto &row = findRowById(id);
    return std::apply([](Id, auto &...cells) { return std::forward_as_tuple(*cells...); }, row);
  }
  /**
   * Get row with the given ID.
   * @param ID index of the desired row
   * @return const reference to row
   * @throws InvalidArgumentException when no row with such ID exists
   */
  auto getRow(Id id) const {
    const auto &row = findRowById(id);
    return std::apply(
        [](Id, const auto &...cells) { return std::forward_as_tuple(const_cast<const decltype(*cells)>(*cells)...); },
        row);
  }
  /**
   * Remove row with the given ID.
   * If no such node exists nothing happens.
   * @param id ID of the node to be removed
   */
  void removeRow(Id id) {
    std::erase_if(rows, [id](const auto &row) { return std::get<0>(row) == id; });
  }

 protected:
  void renderImpl() override {
    if (ImGui::BeginTable(getName().c_str(), ColumnCount, tableFlags, getSize())) {
      if (header.has_value()) {
        std::ranges::for_each(*header, [](const auto &value) { ImGui::TableSetupColumn(value.c_str()); });
        ImGui::TableHeadersRow();
      }

      std::ranges::for_each(rows, [](const auto &row) {
        ImGui::TableNextRow();
        iterateTuple(Visitor{[](auto &column) {
                               ImGui::TableNextColumn();
                               column->render();
                             },
                             [](Id) {}},
                     row);
      });
      ImGui::EndTable();
    }
  }

 private:
  std::optional<StringTableRow<ColumnCount>> header;
  std::vector<Row> rows;
  ImGuiTableFlags tableFlags;

  Id generateId() { return getNext(idGenerator); }

  cppcoro::generator<Id> idGenerator = iota<Id>();

  Factory factory{};

  Row &findRowById(Id id) {
    if (const auto iter = std::ranges::find_if(rows, [id](const auto &row) { return std::get<0>(row) == id; });
        iter != rows.end()) {
      return *iter;
    }
    throw InvalidArgumentException::fmt("Row with id {} not found", id);
  }
};

template<typename Factory, std::derived_from<Renderable>... Cells>
template<bool WasClickable, bool WasObservable, std::size_t TupleIndex>
class TableWithFactory<Factory, Cells...>::RowBuilderFinish {
 public:
  RowBuilderFinish(TableWithFactory &parent, Row &&row) : table(parent), resultRow(std::move(row)) {}
  RowBuilderFinish &addClickListener(std::invocable auto fnc) requires(WasClickable) {
    std::get<TupleIndex>(resultRow)->addClickListener(std::forward<decltype(fnc)>(fnc));
    return *this;
  }

  RowBuilderFinish &addValueListener(auto fnc) requires(WasObservable) {
    using ObservableType = decltype(std::get<TupleIndex>(resultRow)->getValue());
    static_assert(std::invocable<decltype(fnc), ObservableType>, "Listener must be callable with correct value type");
    std::get<TupleIndex>(resultRow)->addValueListener(std::forward<decltype(fnc)>(fnc));
    return *this;
  }

  Id build() {
    table.rows.emplace_back(std::move(resultRow));
    return std::get<0>(resultRow);
  }

 private:
  TableWithFactory &table;
  Row resultRow;
};

template<typename Factory, std::derived_from<Renderable>... Cells>
template<bool WasClickable, bool WasObservable, std::size_t TupleIndex, typename CurrentCell,
         typename... FollowingCells>
class TableWithFactory<Factory, Cells...>::RowBuilder {
 public:
  explicit RowBuilder(TableWithFactory &parent) requires(sizeof...(FollowingCells) == ColumnCount - 1) : table(parent) {
    std::get<0>(resultRow) = table.generateId();
  }
  RowBuilder(TableWithFactory &parent, Row &&row) : table(parent), resultRow(std::move(row)) {}

  template<typename... Args>
  auto operator()(Args &&...args) requires(std::constructible_from<CurrentCell, std::string, Args...>) {
    using namespace std::string_literals;
    constexpr auto IsClickable = std::derived_from<CurrentCell, Clickable>;
    constexpr auto IsObservable = IsValueObservable<CurrentCell>;
    std::get<TupleIndex + 1>(resultRow) = std::make_unique<CurrentCell>(
        "table_row"s + std::to_string(std::get<0>(resultRow)) + "_col" + std::to_string(TupleIndex),
        std::forward<Args>(args)...);
    if constexpr (sizeof...(FollowingCells) > 0) {
      return RowBuilder<IsClickable, IsObservable, TupleIndex + 1, FollowingCells...>{table, std::move(resultRow)};
    } else {
      return RowBuilderFinish<IsClickable, IsObservable, TupleIndex + 1>(table, std::move(resultRow));
    }
  }

  RowBuilder &addClickListener(std::invocable auto fnc) requires(WasClickable) {
    std::get<TupleIndex>(resultRow)->addClickListener(std::forward<decltype(fnc)>(fnc));
    return *this;
  }

  RowBuilder &addValueListener(auto fnc, bool callNow = false) requires(WasObservable) {
    using ObservableType = decltype(std::get<TupleIndex>(resultRow)->getValue());
    static_assert(std::invocable<decltype(fnc), ObservableType>, "Listener must be callable with correct value type");
    std::get<TupleIndex>(resultRow)->addValueListener(std::forward<decltype(fnc)>(fnc), callNow);
    return *this;
  }

 private : TableWithFactory &table;
  Row resultRow{};
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TABLE_H
