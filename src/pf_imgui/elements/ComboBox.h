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
#include <pf_imgui/details/ComboFilter.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <range/v3/view/filter.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

namespace details {
/**
 * @brief Storage for combo box items.
 * @tparam T type to be stored
 */
template<ToStringConvertible T>
struct ComboBoxItemStorage {
  ComboBoxItemStorage(T first) : first(first), second(toString(first)) {}
  T first;
  std::string second;
};
/**
 * @brief Specialisation for std::string.
 * This uses a union so there's no need to change code for this special case, but we don't waste 2 strings neither.
 */
template<>
struct ComboBoxItemStorage<std::string> {
  ComboBoxItemStorage(const std::string &first) : first(first) {}
  ComboBoxItemStorage(const ComboBoxItemStorage &other) : first(other.first) {}
  ComboBoxItemStorage &operator=(const ComboBoxItemStorage &other) {
    first = other.first;
    return *this;
  }
  ~ComboBoxItemStorage() {}
  union {
    std::string first;
    std::string second;
  };
};
}// namespace details

enum class ComboBoxCount {
  Items4 = 1 << 1,
  Items8 = 1 << 2,
  Items20 = 1 << 3,
  ItemsAll = 1 << 4
};

/**
 * @brief A typical combobox with items which can be converted to string.
 *
 * A combobox filled with string convertible type. The element notifies listeners of changes in selection. It also has a filter functionality,
 * which allows for delimiting shown items without the need to rebuild them.
 *
 * @warning If there are multiple items which are the same some unexpected behavior may occur.
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT ComboBox : public ItemElement, public Labellable, public ValueObservable<T>, public Savable {
 public:
  /**
   * Construct Combobox.
   * @param elementName ID of the combobox
   * @param label label drawn next to the element
   * @param previewValue value shown when no item is selected, if nullopt then there's no preview state
   * @param newItems items to use for combobox
   * @param persistent enable state saving to disk
   */
  ComboBox(const std::string &elementName, const std::string &label, std::optional<std::string> previewValue,
           std::ranges::range auto &&newItems, ComboBoxCount showItemCount = ComboBoxCount::Items8,
           Persistent persistent =
               Persistent::No) requires(std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>
                                            &&std::is_default_constructible_v<T> &&std::copy_constructible<T>)
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(), Savable(persistent),
        previewValue(std::move(previewValue)), shownItems(showItemCount) {
    items.reserve(std::ranges::size(newItems));
    std::ranges::copy(newItems, std::back_inserter(items));
  }

  /**
   * Get currently selected item.
   * @return if any item is selected return it, otherwise std::nullopt
   */
  [[nodiscard]] std::optional<T> getSelectedItem() {
    if (selectedItemIndex.has_value()) { return items[*selectedItemIndex].first; }
    return std::nullopt;
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
   * @param itemAsString item to be selected
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
   * @param itemAsString item to be removed
   */
  void removeItem(const std::string &itemAsString) requires(!std::same_as<T, std::string>) {
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
  void setItems(std::ranges::range auto &&newItems) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>) {
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
        ValueObservable<T>::setValueAndNotifyIfChanged(items[idx].first);
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
    auto flags = previewValue.has_value() ? ImGuiComboFlags_{} : ImGuiComboFlags_NoPreview;
    flags |= static_cast<uint>(shownItems);
    const char *previewPtr;
    if (selectedItemIndex.has_value()) {
      previewPtr = items[*selectedItemIndex].second.c_str();
    } else if (previewValue.has_value()) {
      previewPtr = previewValue->c_str();
    } else {
      previewPtr = "";
    }
    if (ImGui::BeginCombo(getLabel().c_str(), previewPtr, flags)) {
      auto cStrItems =
          items | views::transform([](const auto &item) { return std::make_pair(item.first, item.second.c_str()); });
      std::ranges::for_each(cStrItems | views::enumerate
                                | views::filter([this](auto idxPtr) { return filter(idxPtr.second.first); }),
                            [&](auto idxPtr) {
                              const auto [idx, ptr] = idxPtr;
                              auto isSelected = selectedItemIndex.has_value() && *selectedItemIndex == idx;
                              ImGui::Selectable(ptr.second, &isSelected);
                              if (isSelected) {
                                if (!selectedItemIndex.has_value() || *selectedItemIndex != idx) {
                                  ValueObservable<T>::setValueInner(items[idx].first);
                                  ValueObservable<T>::notifyValueChanged();
                                }
                                selectedItemIndex = idx;
                              }
                            });
      ImGui::EndCombo();
    }
  }

 private:
  std::vector<details::ComboBoxItemStorage<T>> items;
  std::optional<std::string> previewValue;
  std::optional<unsigned int> selectedItemIndex = std::nullopt;
  std::function<bool(T)> filter = [](auto) { return true; };
  ComboBoxCount shownItems;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_COMBOBOX_H
