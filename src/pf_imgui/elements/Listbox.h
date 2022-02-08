/**
 * @file ListBox.h
 * @brief ListBox element.
 * @author Petr Flajšingr
 * @date 1.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_LISTBOX_H
#define PF_IMGUI_ELEMENTS_LISTBOX_H

#include <algorithm>
#include <memory>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/CustomListbox.h>
#include <pf_imgui/elements/Selectable.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/resources/ResourceFactory.h>
#include <pf_imgui/unique_id.h>
#include <range/v3/view/addressof.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

namespace details {
template<ToStringConvertible T>
struct PF_IMGUI_EXPORT ListboxRowFactory {
  static inline std::size_t idCounter{};
  const std::string idStart = uniqueId();
  std::unique_ptr<Selectable> operator()(const T &item) {
    return std::make_unique<Selectable>(idStart + std::to_string(idCounter++), makeConstResource(toString(item)));
  }
};
static_assert(CustomItemBoxFactory<ListboxRowFactory<int>, int, Selectable>);
}  // namespace details

/**
 * @brief Container for strings shown as list box and selectable by user.
 *
 * User selection can be observed via listeners.
 * @todo: relax requires
 * @todo: handle pointers as if references
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT Listbox : public CustomListbox<T, Selectable>,
                                public ValueObservable<T>,
                                public Savable,
                                public DragSource<T>,
                                public DropTarget<T> {
  using CustomListbox<T, Selectable>::filteredItems;
  using CustomListbox<T, Selectable>::items;
  using CustomListbox<T, Selectable>::filter;

 public:
  using Factory = details::ListboxRowFactory<T>;
  using CustomListbox<T, Selectable>::getName;
  using CustomListbox<T, Selectable>::getLabel;
  using CustomListbox<T, Selectable>::getSize;
  using CustomListbox<T, Selectable>::setItems;
  using CustomListbox<T, Selectable>::addItem;
  using CustomListbox<T, Selectable>::addItems;
  using CustomListbox<T, Selectable>::removeItem;
  using CustomListbox<T, Selectable>::removeItemIf;
  using CustomListbox<T, Selectable>::setFilter;
  using CustomListbox<T, Selectable>::clearFilter;
  using CustomListbox<T, Selectable>::getItems;
  /**
   * Construct Listbox.
   * @param elementName ID of the element
   * @param label text rendered at the top of the list
   * @param s size of the element
   * @param selectedIdx starting selected id
   * @param persistent enable/disable state saving to disk
   */
  Listbox(
      const std::string &elementName, std::unique_ptr<Resource<std::string>> label, Size s = Size::Auto(),
      std::optional<int> selectedIdx = std::nullopt,
      Persistent persistent = Persistent::No) requires(std::is_default_constructible_v<T> &&std::copy_constructible<T>)
      : CustomListbox<T, Selectable>(elementName, std::move(label), Factory{}, s), ValueObservable<T>(),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), selectedItemIndex(selectedIdx) {}

  /**
   * Add item to the end of the list.
   * @param item item to be added
   * @param selected when selected the item is activated in the same way as if the user clicked on it
   * and observers are notified
   */
  Selectable &addItem(const T &item, Selected selected) {
    auto &result = addItem(item);
    if (selected == Selected::Yes) { setSelectedItem(item); }
    return result;
  }
  /**
   * Get and item currently selected by the user.
   * @param data
   */
  [[nodiscard]] std::optional<std::reference_wrapper<T>> getSelectedItem() const {
    if (selectedItemIndex.has_value()) { return filteredItems[*selectedItemIndex]->first; }
    return std::nullopt;
  }

  /**
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemToSelect item to select
   */
  void setSelectedItem(const T &itemToSelect) requires(!std::same_as<T, std::string>) {
    if constexpr (std::equality_comparable<T>) {
      if (const auto iter =
              std::ranges::find_if(items, [&itemToSelect](const auto &item) { return item->first == itemToSelect; });
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
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemAsString string representation of item to select
   */
  void setSelectedItem(const std::string &itemAsString) {
    if (const auto iter = std::ranges::find_if(
            filteredItems, [itemAsString](const auto &item) { return item->second->getLabel().get() == itemAsString; });
        iter != filteredItems.end()) {
      const auto index = std::distance(filteredItems.begin(), iter);
      setSelectedItemByIndex(index);
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

 protected:
  using AllColorCustomizable::setColorStack;
  using AllStyleCustomizable::setStyleStack;
  void renderImpl() override {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    if (ImGui::BeginListBox(getLabel().get().c_str(), getSize().asImVec())) {
      RAII end{[] { ImGui::EndListBox(); }};
      std::ranges::for_each(filteredItems | ranges::views::enumerate, [this](const auto &itemIdx) {
        const auto &[idx, item] = itemIdx;
        item->second->render();
        if (item->second->getValue()) { setSelectedItemByIndex(idx); }
      });
    }
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) { addItem(*drop); }
    if (selectedItemIndex.has_value()) { DragSource<T>::drag(filteredItems[*selectedItemIndex]->first); }
  }

  void refilterItems() override {
    auto origItem = selectedItemIndex.has_value() ? filteredItems[*selectedItemIndex] : nullptr;
    filteredItems = items | std::views::filter([this](auto &item) { return filter(item.first); })
        | std::views::transform([](auto &item) { return &item; }) | ranges::to_vector;
    selectedItemIndex = std::nullopt;
    if (origItem != nullptr) {
      for (const auto &[idx, item] : filteredItems | ranges::views::enumerate) {
        if (item == origItem) {
          selectedItemIndex = static_cast<int>(idx);
          break;
        }
      }
    }
  }

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
      result.insert_or_assign("selected", selectedItem->second->getLabel().get());
    }
    return result;
  }

 private:
  std::optional<std::size_t> selectedItemIndex = std::nullopt;
};
#ifdef PF_IMGUI_ENABLE_EXTERN_TEMPLATE
extern template class Listbox<std::string>;
#endif

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_LISTBOX_H
