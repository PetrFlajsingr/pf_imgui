//
// Created by petr on 6/4/21.
//

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
 * @todo: combine CustomCombobox and Customlistbox to a base class:
 *  CustomElementContainer -> CustomCombobox/CustomListbox
 *  Selectable or smt
 *
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
      const auto idx = **src["selected"].as_integer();
      if (static_cast<std::size_t>(idx) < items.size()) {
        selectedItemIndex = idx;
        ValueObservable<T>::setValueAndNotifyIfChanged(items[idx].first);
      }
    }
  }
  toml::table serialize_impl() override {//FIXME: save string representation of selected item
    auto result = toml::table{};
    if (selectedItemIndex.has_value()) {
      const auto selectedItem = filteredItems[*selectedItemIndex];
      auto itemsWithIndices = items | ranges::views::addressof | ranges::views::enumerate | ranges::to_vector;
      const auto indexInAllItems =
          static_cast<int>(std::ranges::find_if(itemsWithIndices, [selectedItem](const auto &itemInfo) {
            return itemInfo.second->second.get() == selectedItem->second.get();
          })->first);
      result.insert_or_assign("selected", indexInAllItems);
    }
    return result;
  }
  void renderImpl() override {
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
