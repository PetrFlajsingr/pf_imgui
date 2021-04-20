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
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/ValueObservable.h>
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
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT ListBox : public ItemElement, public Labellable, public ValueObservable<T> {
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
          std::ranges::range auto &&newItems = std::vector<T>{}, int selectedIdx = 0,
          int heightInItems = -1) requires(std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>
                                               &&std::is_default_constructible_v<T> &&std::copy_constructible<T>)
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(), currentItemIdx(selectedIdx),
        height(heightInItems) {
    std::ranges::copy(newItems, std::back_inserter(items));
  }

  /**
   * Add item to the end of the list.
   * @param item item to be added
   */
  void addItem(const T &item) { items.emplace_back(item); }
  /**
   * Add items to the end of the list.
   * @param data items to be added
   */
  void addItems(std::ranges::range auto &&data) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(data)>, T>) {
    std::ranges::copy(data, std::back_inserter(items));
  }
  /**
   * Overwrite current items with the ones provided.
   * @param data new items
   */
  void setItems(std::ranges::range auto &&data) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(data)>, T>) {
    items.clear();
    currentItemIdx = 0;
    std::ranges::copy(data, std::back_inserter(items));
  }

  /**
   * Get and item currently selected by the user.
   * @param data
   */
  [[nodiscard]] const T &getSelectedItem() const { return items[currentItemIdx].first; }

  /**
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemToSelect item to select
   */
  void setSelectedItem(const T &itemToSelect) {
    const auto itemAsString = toString(itemToSelect);
    setSelectedItem(itemAsString);
  }

  /**
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemAsString string representation of item to select
   */
  void setSelectedItem(const std::string &itemAsString) {
    if (const auto iter =
            std::ranges::find_if(items, [itemAsString](const auto &item) { return item.second == itemAsString; });
        iter != items.end()) {
      const auto index = std::distance(items.begin(), iter);
      currentItemIdx = index;
    }
  }

 protected:
  void renderImpl() override {
    const auto cStrItems =
        items | ranges::views::transform([](const auto &str) { return str.second.c_str(); }) | ranges::to_vector;
    if (ImGui::ListBox(getLabel().c_str(), &currentItemIdx, cStrItems.data(), cStrItems.size(), height)) {
      ValueObservable<T>::setValueInner(items[currentItemIdx].first);
      ValueObservable<T>::notifyValueChanged();
    }
  }

 private:
  std::vector<details::ListBoxItemStorage<T>> items;
  int currentItemIdx = 0;
  int height = -1;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_LISTBOX_H
