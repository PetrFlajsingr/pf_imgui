/**
 * @file ListBox.h
 * @brief ListBox element.
 * @author Petr Flajšingr
 * @date 1.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_LISTBOX_H
#define PF_IMGUI_ELEMENTS_LISTBOX_H

#include <algorithm>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <range/v3/view/addressof.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

namespace details {
/**
 * @brief Storage for list box items.
 * @tparam T type to be stored
 */
template<ToStringConvertible T>
struct ListBoxItemStorage {
  ListBoxItemStorage(T first) : first(first), second(toString(first)) {}
  T first;
  std::string second;
};
/**
 * @brief Specialisation for std::string.
 * This uses a union so there's no need to change code for this special case, but we don't waste 2 strings neither.
 */
template<>
struct ListBoxItemStorage<std::string> {
  ListBoxItemStorage(const std::string &first) : first(first) {}
  ListBoxItemStorage(const ListBoxItemStorage &other) : first(other.first) {}
  ListBoxItemStorage &operator=(const ListBoxItemStorage &other) {
    first = other.first;
    return *this;
  }
  ~ListBoxItemStorage() {}
  union {
    std::string first;
    std::string second;
  };
};
}// namespace details

/**
 * @brief Container for strings shown as list box and selectable by user.
 *
 * User selection can be observed via listeners.
 *
 * @todo: custom listbox
 * @todo: handle pointers as values
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT ListBox : public ItemElement,
                                public Labellable,
                                public ValueObservable<T>,
                                public Savable,
                                public DragSource<T>,
                                public DropTarget<T> {
 public:
  /**
   * Construct ListBox.
   * @param elementName ID of the element
   * @param label text rendered at the top of the list
   * @param newItems starting items in the list
   * @param selectedIdx starting selected id
   * @param heightInItems items to show before scroll is enabled - -1 shows all
   */
  ListBox(const std::string &elementName, const std::string &label,
          std::ranges::range auto &&newItems = std::vector<T>{}, std::optional<int> selectedIdx = std::nullopt,
          int heightInItems = -1,
          Persistent persistent =
              Persistent::No) requires(std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>
                                           &&std::is_default_constructible_v<T> &&std::copy_constructible<T>)
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), selectedItemIndex(selectedIdx),
        height(heightInItems) {
    std::ranges::copy(newItems, std::back_inserter(items));
    refilterItems();
  }

  /**
   * Add item to the end of the list.
   * @param item item to be added
   */
  void addItem(const T &item) {
    items.emplace_back(item);
    refilterItems();
  }
  /**
   * Add items to the end of the list.
   * @param data items to be added
   */
  void addItems(std::ranges::range auto &&data) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(data)>, T>) {
    std::ranges::copy(data, std::back_inserter(items));
    refilterItems();
  }
  /**
   * Overwrite current items with the ones provided.
   * @param data new items
   */
  void setItems(std::ranges::range auto &&data) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(data)>, T>) {
    items.clear();
    selectedItemIndex = std::nullopt;
    std::ranges::copy(data, std::back_inserter(items));
    refilterItems();
  }

  /**
   * Get and item currently selected by the user.
   * @param data
   */
  [[nodiscard]] std::optional<T> getSelectedItem() const {
    if (selectedItemIndex.has_value()) { return filteredItems[*selectedItemIndex]->first; }
    return std::nullopt;
  }

  /**
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemToSelect item to select
   */
  void setSelectedItem(const T &itemToSelect) {
    if constexpr (std::equality_comparable<T>) {
      if (const auto iter = std::ranges::find_if(
              filteredItems, [&itemToSelect](const auto &item) { return item->first == itemToSelect; });
          iter != filteredItems.end()) {
        const auto index = std::distance(filteredItems.begin(), iter);
        setSelectedItemByIndex(index);
      }
    } else {
      const auto itemAsString = toString(itemToSelect);
      setSelectedItem(itemAsString);
    }
  }

  /**
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemAsString string representation of item to select
   */
  void setSelectedItem(const std::string &itemAsString) {
    if (const auto iter = std::ranges::find_if(
            filteredItems, [itemAsString](const auto &item) { return item->second == itemAsString; });
        iter != filteredItems.end()) {
      const auto index = std::distance(filteredItems.begin(), iter);
      setSelectedItemByIndex(index);
      selectedItemIndex = index;
    }
  }

  void setSelectedItemByIndex(std::size_t index) {
    assert(index < items.size());
    if (index != selectedItemIndex) {
      selectedItemIndex = index;
      ValueObservable<T>::setValueInner(filteredItems[currentItemIdx]->first);
      ValueObservable<T>::notifyValueChanged();
    }
  }

  /**
   * Set a predicate that filters which items are displayed to the user.
   * @param filterFnc predicate returning true for items which should be shown to the user
   */
  void setFilter(std::predicate<const T &> auto filterFnc) {
    filter = filterFnc;
    refilterItems();
  }
  /**
   * Remove item filter.
   */
  void clearFilter() {
    filter = [](auto) { return true; };
    refilterItems();
  }

  /**
   * Get all items in ListBox.
   * @return all items in ListBox
   */
  [[nodiscard]] auto getItems() {
    return items | std::views::transform([](auto &item) -> T & { return item.first; });
  }
  /**
   * Get all items in ListBox.
   * @return all items in ListBox
   */
  [[nodiscard]] auto getItems() const {
    return items | std::views::transform([](auto &item) -> const T & { return item.first; });
  }

  /**
   * Remove item by value.
   * @param itemToRemove item to remove
   */
  void removeItem(const T &itemToRemove) requires(std::equality_comparable<T> && !std::same_as<T, std::string>) {
    std::erase_if(items, [itemToRemove](const auto &item) { return item.first == itemToRemove; });
    reloadItems();
  }

  /**
   * Remove item by string representation
   * @param itemToRemove string representation of item to remove
   */
  void removeItem(const std::string &itemToRemove) {
    std::erase_if(items, [itemToRemove](const auto &item) { return itemToRemove == item.first; });
    reloadItems();
  }

  /**
   * Reload items. This should be used after external change, for example when changing items via getItems().
   */
  void reloadItems() {
    auto oldItems = items;
    items.clear();
    std::ranges::for_each(oldItems, [this](auto &item) { items.emplace_back(item); });
    refilterItems();
  }

 protected:
  void renderImpl() override {
    const auto cStrItems = filteredItems | ranges::views::transform([](const auto &str) { return str->second.c_str(); })
        | ranges::to_vector;
    auto currentItemIdx = selectedItemIndex.template value_or(-1);
    if (ImGui::ListBox(getLabel().c_str(), &currentItemIdx, cStrItems.data(), cStrItems.size(), height)) {
      ValueObservable<T>::setValueInner(filteredItems[currentItemIdx]->first);
      ValueObservable<T>::notifyValueChanged();
      selectedItemIndex = currentItemIdx;
    }
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) { addItem(*drop); }
    if (selectedItemIndex.has_value()) { DragSource<T>::drag(filteredItems[*selectedItemIndex]->first); }
  }

  void unserialize_impl(const toml::table &src) override {
    if (src.contains("selected")) {
      const auto idx = *src["selected"].value<int>();
      if (static_cast<std::size_t>(idx) < items.size()) {
        selectedItemIndex = idx;
        ValueObservable<T>::setValueAndNotifyIfChanged(items[idx].first);
      }
    }
  }

  toml::table serialize_impl() override {
    auto result = toml::table{};
    if (selectedItemIndex.has_value()) {
      const auto selectedItem = *filteredItems[*selectedItemIndex];
      auto itemsWithIndices = ranges::views::enumerate(items) | ranges::to_vector;
      const auto indexInAllItems =
          static_cast<int>(std::ranges::find_if(itemsWithIndices, [selectedItem](const auto &itemInfo) {
                             return itemInfo.second.first == selectedItem.first;
                           })->first);
      result.insert_or_assign("selected", indexInAllItems);
    }
    return result;
  }

 private:
  void refilterItems() {
    filteredItems = items | ranges::views::filter([this](const auto &item) { return filter(item.first); })
        | ranges::views::addressof | ranges::to_vector;
    selectedItemIndex = std::nullopt;
  }
  std::vector<details::ListBoxItemStorage<T>> items;
  std::vector<details::ListBoxItemStorage<T> *> filteredItems;
  std::optional<int> selectedItemIndex = std::nullopt;
  int height = -1;
  std::function<bool(const T &)> filter = [](auto) { return true; };
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_LISTBOX_H
