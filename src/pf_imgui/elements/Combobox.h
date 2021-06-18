/**
 * @file Combobox.h
 * @brief Selectable combobox.
 * @author Petr Flajšingr
 * @date 4.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_COMBOBOX_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_COMBOBOX_H

#include <pf_imgui/elements/CustomCombobox.h>
#include <pf_imgui/elements/Selectable.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <range/v3/view/addressof.hpp>

namespace pf::ui::ig {

namespace details {
template<ToStringConvertible T>
struct ComboboxRowFactory {
  static inline cppcoro::generator<std::size_t> idGenerator = iota<std::size_t>();
  const std::string idStart = uniqueId();
  std::unique_ptr<Selectable> operator()(const T &item) {
    return std::make_unique<Selectable>(idStart + std::to_string(getNext(idGenerator)), toString(item));
  }
};
static_assert(CustomItemBoxFactory<ComboboxRowFactory<int>, int, Selectable>);
}// namespace details

/**
 * @brief A typical combobox with items which can be converted to string.
 *
 * A combobox filled with string convertible type. The element notifies listeners of changes in selection. It also has a filter functionality,
 * which allows for delimiting shown items without the need to rebuild them.
 *
 * @warning If there are multiple items which are the same some unexpected behavior may occur.
 *
 * @todo: keyboard controls on focus
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT Combobox : public CustomCombobox<T, Selectable>,
                                 public ValueObservable<T>,
                                 public Savable,
                                 public DragSource<T> {
  using CustomCombobox<T, Selectable>::items;
  using CustomCombobox<T, Selectable>::filteredItems;
  using CustomCombobox<T, Selectable>::flags;

 public:
  using CustomCombobox<T, Selectable>::addItem;
  using CustomCombobox<T, Selectable>::addItems;
  using CustomCombobox<T, Selectable>::setItems;
  using CustomCombobox<T, Selectable>::removeItem;
  using CustomCombobox<T, Selectable>::removeItemIf;
  using CustomCombobox<T, Selectable>::setFilter;
  using CustomCombobox<T, Selectable>::clearFilter;
  using CustomCombobox<T, Selectable>::getItems;
  using CustomCombobox<T, Selectable>::setPreviewValue;
  using CustomCombobox<T, Selectable>::getPreviewValue;
  using CustomCombobox<T, Selectable>::getShownItemCount;
  using CustomCombobox<T, Selectable>::setShownItemCount;
  using CustomCombobox<T, Selectable>::getLabel;
  using CustomCombobox<T, Selectable>::getName;
  using CustomCombobox<T, Selectable>::checkClose;
  /**
   * Construct Combobox.
   * @param elementName ID of the element
   * @param label text rendered next to the element
   * @param prevValue preview value
   * @param newItems items to be converted and rendered
   * @param showItemCount amount of items shown when open
   * @param persistent enable/disable disk state saving
   */
  Combobox(const std::string &elementName, const std::string &label, const std::string &prevValue,
           std::ranges::range auto &&newItems, ComboBoxCount showItemCount = ComboBoxCount::Items8,
           Persistent persistent =
               Persistent::No) requires(std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>
                                            &&std::is_default_constructible_v<T> &&std::copy_constructible<T>)
      : CustomCombobox<T, Selectable>(elementName, label, details::ComboboxRowFactory<T>{}, prevValue, showItemCount),
        ValueObservable<T>(), Savable(persistent), DragSource<T>(false) {
    addItems(std::forward<decltype(newItems)>(newItems));
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
  void setSelectedItem(T &itemToSelect) {
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
   * Set selected item by its string value. If no such item is found the selection is cancelled.
   * @param itemAsString item to be selected
   */
  void setSelectedItem(const std::string &itemAsString) {
    if (const auto iter =
            std::ranges::find_if(items, [itemAsString](const auto &item) { return item.second == itemAsString; });
        iter != items.end()) {
      const auto index = std::distance(items.begin(), iter);
      setSelectedItemByIndex(index);
    } else {
      cancelSelection();
    }
  }
  /**
   * Select item by its index.
   * @param index index to select
   */
  void setSelectedItemByIndex(std::size_t index) {
    assert(index < items.size());
    if (index != selectedItemIndex) {
      if (selectedItemIndex.has_value()) { filteredItems[*selectedItemIndex]->second->setValue(false); }
      selectedItemIndex = index;
      filteredItems[*selectedItemIndex]->second->setValue(true);
      ValueObservable<T>::setValueInner(filteredItems[index]->first);
      ValueObservable<T>::notifyValueChanged();
    }
  }
  /**
   * Cancel selection and show previewValue.
   */
  void cancelSelection() { selectedItemIndex = std::nullopt; }

 protected:
  void unserialize_impl(const toml::table &src) override {
    if (src.contains("selected")) {
      const auto selectedItemAsString = *src["selected"].value<std::string>();
      for (const auto &[idx, item] : items | ranges::views::enumerate) {
        if (item.second->getLabel() == selectedItemAsString) {
          selectedItemIndex = idx;
          items[idx].second->setValue(true);
          ValueObservable<T>::setValueAndNotifyIfChanged(items[idx].first);
          break;
        }
      }
    }
  }

  toml::table serialize_impl() override {
    auto result = toml::table{};
    if (selectedItemIndex.has_value()) {
      const auto selectedItem = filteredItems[*selectedItemIndex];
      result.insert_or_assign("selected", selectedItem->second->getLabel());
    }
    return result;
  }

  void renderImpl() override {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    const char *previewPtr;
    if (selectedItemIndex.has_value()) {
      previewPtr = filteredItems[*selectedItemIndex]->second->getLabel().c_str();
    } else {
      previewPtr = getPreviewValue().c_str();
    }
    if (ImGui::BeginCombo(getLabel().c_str(), previewPtr, *flags)) {
      checkClose();
      std::ranges::for_each(filteredItems | ranges::views::enumerate, [this](const auto &itemIdx) {
        const auto &[idx, item] = itemIdx;
        item->second->render();
        if (item->second->getValue()) { setSelectedItemByIndex(idx); }
      });
      ImGui::EndCombo();
    }
    if (selectedItemIndex.has_value()) { DragSource<T>::drag(filteredItems[*selectedItemIndex]->first); }
  }

 private:
  std::optional<unsigned int> selectedItemIndex = std::nullopt;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_COMBOBOX_H
