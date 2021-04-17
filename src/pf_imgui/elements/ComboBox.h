/**
 * @file ComboBox.h
 * @brief ComboBox element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_COMBOBOX_H
#define PF_IMGUI_ELEMENTS_COMBOBOX_H

#include <optional>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <range/v3/view/filter.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

///**
// * @brief A typical combobox with string items.
// *
// * A combobox filled with strings. The element notifies listeners of changes in selection. It also has a filter functionality,
// * which allows for delimiting shown items without the need to rebuild them.
// *
// * @warning If there are multiple items which are the same some unexpected behavior may occur.
// *
// * @todo: built-in filter
// * @todo: generic item
// */
//class PF_IMGUI_EXPORT ComboBox : public ItemElement,
//                                 public Labellable,
//                                 public ValueObservable<std::string_view>,
//                                 public Savable {
// public:
//  /**
//   * Construct Combobox.
//   * @param elementName ID of the combobox
//   * @param label label drawn next to the element
//   * @param previewValue value shown when no item is selected
//   * @param items
//   * @param persistent
//   */
//  ComboBox(const std::string &elementName, const std::string &label, std::string previewValue,
//           std::vector<std::string> items, Persistent persistent = Persistent::No);
//
//  /**
//   * Get currently selected item.
//   * @return if any item is selected return it, otherwise std::nullopt
//   */
//  [[nodiscard]] std::optional<std::string_view> getSelectedItem();
//  /**
//   * Set selected item. If no such item is found the selection is cancelled.
//   * @param item item to be selected
//   */
//  void setSelectedItem(const std::string &item);
//  /**
//   * Cancel selection and show previewValue.
//   */
//  void cancelSelection();
//  /**
//   * Remove this item from the Combobox. If no such item exists nothing happens.
//   * @param item item to be removed
//   */
//  void removeItem(const std::string &item);
//  /**
//   * Add a new item to the end of the items.
//   * @param item item to be added
//   */
//  void addItem(const std::string &item);
//
//  /**
//   * Get all items.
//   * @return items in the combobox
//   */
//  [[nodiscard]] const std::vector<std::string> &getItems() const;
//  /**
//   * Set new items, overwriting the old ones.
//   * @param newItems items to set
//   */
//  void setItems(std::vector<std::string> newItems);
//
//  /**
//   * Set a predicate that filters which items are displayed to the user.
//   * @param filterFnc predicate returning true for items which should be shown to the user
//   */
//  void setFilter(std::predicate<std::string_view> auto filterFnc) { filter = filterFnc; }
//  /**
//   * Remove item filter.
//   */
//  void clearFilter();
//
// protected:
//  void unserialize_impl(const toml::table &src) override;
//  toml::table serialize_impl() override;
//  void renderImpl() override;
//
// private:
//  std::vector<std::string> items;
//  std::string previewValue;
//  std::optional<unsigned int> selectedItemIndex = std::nullopt;
//  std::function<bool(std::string_view)> filter = [](auto) { return true; };
//};

/**
 * @brief A typical combobox with items which can be converted to string.
 *
 * A combobox filled with string convertible type. The element notifies listeners of changes in selection. It also has a filter functionality,
 * which allows for delimiting shown items without the need to rebuild them.
 *
 * @warning If there are multiple items which are the same some unexpected behavior may occur.
 *
 * @todo: built-in filter
 * @todo: optimize for strings
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT ComboBox : public ItemElement, public Labellable, public ValueObservable<T>, public Savable {
 public:
  /**
   * Construct Combobox.
   * @param elementName ID of the combobox
   * @param label label drawn next to the element
   * @param previewValue value shown when no item is selected
   * @param newItems items to use for combobox
   * @param persistent enable state saving to disk
   */
   ComboBox(const std::string &elementName, const std::string &label, std::string previewValue,
            const std::ranges::range auto &newItems,
            Persistent persistent =
                Persistent::No) requires(std::same_as<std::ranges::range_value_t<decltype(newItems)>, T>
                                             &&std::is_default_constructible_v<T> &&std::copy_constructible<T>)
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(), Savable(persistent),
        previewValue(std::move(previewValue)) {
    items.reserve(std::ranges::size(newItems));
    std::ranges::transform(newItems, std::back_inserter(items),
                           [](const auto &item) { return std::make_pair(item, toString(item)); });
  }

  /**
   * Get currently selected item.
   * @return if any item is selected return it, otherwise std::nullopt
   */
  [[nodiscard]] std::optional<T> getSelectedItem() {
    return selectedItemIndex.has_value() ? items[*selectedItemIndex] : std::nullopt;
  }
  /**
   * Set selected item. If no such item is found the selection is cancelled.
   * @param item item to be selected
   */
  void setSelectedItem(T &item) {
    const auto itemAsString = toString(item);
    setSelectedItem(itemAsString);
  }
  /**
   * Set selected item by its string value. If no such item is found the selection is cancelled.
   * @param item item to be selected
   */
  void setSelectedItem(const std::string &itemAsString) {
    if (const auto iter =
            std::ranges::find_if(items, [itemAsString](const auto &item) { return item.second == itemAsString; });
        iter != items.end()) {
      const auto index = std::distance(items.begin(), iter);
      selectedItemIndex = index;
    } else {
      cancelSelection();
    }
  }
  /**
   * Cancel selection and show previewValue.
   */
  void cancelSelection() { selectedItemIndex = std::nullopt; }
  /**
   * Remove this item from the Combobox. If no such item exists nothing happens.
   * @param item item to be removed
   */
  void removeItem(const T &item) {
    const auto itemAsString = toString(item);
    removeItem(itemAsString);
  }
  /**
   * Remove this item from the Combobox by its string value. If no such item exists nothing happens.
   * @param item item to be removed
   */
  void removeItem(const std::string &itemAsString) {
    using namespace std::string_literals;
    if (const auto iter =
            std::ranges::find_if(items, [itemAsString](const auto &item) { return item.second == itemAsString; });
        iter != items.end()) {
      const auto isAnyItemSelected = selectedItemIndex.has_value();
      const auto selectedItem = isAnyItemSelected ? items[*selectedItemIndex] : ""s;
      items.erase(iter);
      selectedItemIndex = std::nullopt;
      if (isAnyItemSelected) { setSelectedItem(selectedItem); }
    }
  }
  /**
   * Add a new item to the end of the items.
   * @param item item to be added
   */
  void addItem(const T &item) { items.template emplace_back(item, toString(item)); }

  /**
   * Get all items.
   * @return items in the combobox
   */
  [[nodiscard]] auto getItems() const {
    return items | std::views::transform([](const auto &item) -> const T & { return item.first; });
  }
  /**
   * Set new items, overwriting the old ones.
   * @param newItems items to set
   */
  void setItems(const std::ranges::range auto &newItems) requires(
      std::same_as<std::ranges::range_value_t<decltype(newItems)>, T>) {
    items.clear();
    std::ranges::copy(newItems, std::back_inserter(items));
  }

  /**
   * Set a predicate that filters which items are displayed to the user.
   * @param filterFnc predicate returning true for items which should be shown to the user
   */
  void setFilter(std::predicate<std::string_view> auto filterFnc) { filter = filterFnc; }
  /**
   * Remove item filter.
   */
  void clearFilter() {
    filter = [](auto) { return true; };
  }

 protected:
  void unserialize_impl(const toml::table &src) override {
    if (src.contains("selected")) {
      const auto idx = **src["selected"].as_integer();
      if (static_cast<std::size_t>(idx) < items.size()) {
        selectedItemIndex = idx;
        setValueAndNotifyIfChanged(items[idx]);
      }
    }
  }
  toml::table serialize_impl() override {
    auto result = toml::table{};
    if (selectedItemIndex.has_value()) { result.insert_or_assign("selected", *selectedItemIndex); }
    return result;
  }
  void renderImpl() override {
    using namespace ranges;
    if (ImGui::BeginCombo(getLabel().c_str(),
                          selectedItemIndex.has_value() ? items[*selectedItemIndex].c_str() : previewValue.c_str())) {
      auto cStrItems = items | views::transform([](const auto &item) { return item.second.c_str(); });
      std::ranges::for_each(cStrItems | views::enumerate
                                | views::filter([this](auto idxPtr) { return filter(idxPtr.second); }),
                            [&](auto idxPtr) {
                              const auto [idx, ptr] = idxPtr;
                              auto isSelected = selectedItemIndex.has_value() && *selectedItemIndex == idx;
                              ImGui::Selectable(ptr, &isSelected);
                              if (isSelected) {
                                if (!selectedItemIndex.has_value() || *selectedItemIndex != idx) {
                                  ValueObservable<T>::setValueInner(items[idx]);
                                  ValueObservable<T>::notifyValueChanged();
                                }
                                selectedItemIndex = idx;
                              }
                            });
      ImGui::EndCombo();
    }
  }

 private:
  std::vector<std::pair<T, std::string>> items;
  std::string previewValue;
  std::optional<unsigned int> selectedItemIndex = std::nullopt;
  std::function<bool(T)> filter = [](auto) { return true; };
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_COMBOBOX_H
