/**
* @file Combobox.h
* @brief Selectable combobox.
* @author Petr Flajšingr
* @date 4.6.21
*/

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_COMBOBOX_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_COMBOBOX_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/CustomCombobox.h>
#include <pf_imgui/elements/Selectable.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <range/v3/view/addressof.hpp>
#include <range/v3/view/enumerate.hpp>
#include <string>
#include <utility>

namespace pf::ui::ig {

namespace details {
template<ToStringConvertible T>
struct PF_IMGUI_EXPORT ComboboxRowFactory {
  static inline std::size_t idCounter{};
  const std::string idStart = uniqueId();
  std::unique_ptr<Selectable> operator()(const T &item) {
    return std::make_unique<Selectable>(idStart + std::to_string(idCounter++), toString(item));
  }
};
static_assert(CustomItemBoxFactory<ComboboxRowFactory<int>, int, Selectable>);
}  // namespace details

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
  using CustomComboboxBase = CustomCombobox<T, Selectable>;
  using CustomComboboxBase::filteredItems;
  using CustomComboboxBase::flags;
  using CustomComboboxBase::items;

 public:
  using CustomComboboxBase::addItem;
  using CustomComboboxBase::addItems;
  using CustomComboboxBase::checkClose;
  using CustomComboboxBase::clearFilter;
  using CustomComboboxBase::getItems;
  using CustomComboboxBase::getLabel;
  using CustomComboboxBase::getName;
  using CustomComboboxBase::getPreviewValue;
  using CustomComboboxBase::getShownItemCount;
  using CustomComboboxBase::removeItem;
  using CustomComboboxBase::removeItemIf;
  using CustomComboboxBase::setFilter;
  using CustomComboboxBase::setItems;
  using CustomComboboxBase::setPreviewValue;
  using CustomComboboxBase::setShownItemCount;

  /**
    * @brief Struct for construction of Combobox.
    */
  struct Config {
    using Parent = Combobox;
    std::string_view name;                                /*!< Unique name of the element */
    std::string_view label;                               /*!< Text rendered next to the Combobox */
    std::string preview;                                  /*!< Preview value shown when no item is selected */
    ComboBoxCount shownItemCount = ComboBoxCount::Items8; /*!< Amount of items shown when Combobox is open */
    Persistent persistent = Persistent::No;              /*!< Allow state saving to disk */
  };
  /**
   * Construct Combobox
   * @param config construction args @see Combobox::Config
   */
  explicit Combobox(Config &&config)
      : CustomComboboxBase(std::string{config.name}, std::string{config.label}, details::ComboboxRowFactory<T>{},
                           std::string{config.preview}, config.shownItemCount),
        ValueObservable<T>(), Savable(config.persistent), DragSource<T>(false) {}
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
      : CustomComboboxBase(elementName, label, details::ComboboxRowFactory<T>{}, prevValue, showItemCount),
        ValueObservable<T>(), Savable(persistent), DragSource<T>(false) {
    addItems(std::forward<decltype(newItems)>(newItems));
  }
  /**
   * Get currently selected item.
   * @return if any item is selected return it, otherwise std::nullopt
   */
  [[nodiscard]] std::optional<T> getSelectedItem() const {
    if (selectedItemIndex.has_value()) { return items[*selectedItemIndex].first; }
    return std::nullopt;
  }
  /**
   * Set selected item. If no such item is found the selection is cancelled.
   * @param item item to be selected
   */
  void setSelectedItem(const T &itemToSelect) requires(!std::same_as<T, std::string>) {
    if constexpr (std::equality_comparable<T>) {
      if (const auto iter =
              std::ranges::find_if(items, [&itemToSelect](const auto &item) { return item.first == itemToSelect; });
          iter != items.end()) {
        const auto index = std::distance(items.begin(), iter);
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
    if (const auto iter = std::ranges::find_if(
            items, [itemAsString](const auto &item) { return item.second->getLabel() == itemAsString; });
        iter != items.end()) {
      const auto index = std::ranges::distance(items.begin(), iter);
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
      selectedItemIndex = static_cast<int>(index);
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
  using AllColorCustomizable::setColorStack;
  using AllStyleCustomizable::setStyleStack;
  void unserialize_impl(const toml::table &src) override {
    if (auto selectedValIter = src.find("selected"); selectedValIter != src.end()) {
      if (auto selectedVal = selectedValIter->second.value<std::string>(); selectedVal.has_value()) {
        setSelectedItem(selectedVal.value());
      }
    }
  }

  [[nodiscard]] toml::table serialize_impl() const override {
    auto result = toml::table{};
    if (selectedItemIndex.has_value()) {
      const auto selectedItem = filteredItems[*selectedItemIndex];
      result.insert_or_assign("selected", selectedItem->second->getLabel());
    }
    return result;
  }

  void renderImpl() override {
    [[maybe_unused]] auto colorStyle = setColorStack();
    [[maybe_unused]] auto style = setStyleStack();
    const char *previewPtr;
    if (selectedItemIndex.has_value()) {
      previewPtr = filteredItems[*selectedItemIndex]->second->getLabel().c_str();
    } else {
      previewPtr = getPreviewValue().c_str();
    }
    if (ImGui::BeginCombo(getLabel().c_str(), previewPtr, *flags)) {
      RAII end{ImGui::EndCombo};
      checkClose();
      std::ranges::for_each(filteredItems | ranges::views::enumerate, [this](const auto &itemIdx) {
        const auto &[idx, item] = itemIdx;
        item->second->render();
        if (item->second->getValue()) { setSelectedItemByIndex(idx); }
      });
    }
    if (selectedItemIndex.has_value()) { DragSource<T>::drag(filteredItems[*selectedItemIndex]->first); }
  }

 private:
  std::optional<unsigned int> selectedItemIndex = std::nullopt;
};


extern template class Combobox<std::string>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_COMBOBOX_H
