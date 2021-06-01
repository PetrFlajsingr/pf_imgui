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
#include <pf_common/coroutines/Sequence.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/CustomListbox.h>
#include <pf_imgui/elements/Selectable.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/unique_id.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

namespace details {
template<ToStringConvertible T>
struct ListboxRowFactory {
  cppcoro::generator<std::size_t> idGenerator = iota<std::size_t>();
  const std::string idStart = uniqueId();
  std::unique_ptr<Selectable> operator()(const T &item) {
    return std::make_unique<Selectable>(idStart + std::to_string(getNext(idGenerator), toString(item)));
  }
};
static_assert(CustomListboxRowFactory<ListboxRowFactory<int>>);
}// namespace details

/**
 * @brief Container for strings shown as list box and selectable by user.
 *
 * User selection can be observed via listeners.
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT Listbox2 : public CustomListBox<T>,
                                 public ValueObservable<T>,
                                 public Savable,
                                 public DragSource<T>,
                                 public DropTarget<T> {
 public:
  using Factory = details::ListboxRowFactory<T>;
  /**
   * Construct Listbox.
   * @param elementName ID of the element
   * @param label text rendered at the top of the list
   * @param newItems starting items in the list
   * @param selectedIdx starting selected id
   * @param heightInItems items to show before scroll is enabled - -1 shows all
   */
  Listbox2(const std::string &elementName, const std::string &label,
           std::ranges::range auto &&newItems = std::vector<T>{}, std::optional<int> selectedIdx = std::nullopt,
           int heightInItems = -1,
           Persistent persistent =
               Persistent::No) requires(std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>
                                            &&std::is_default_constructible_v<T> &&std::copy_constructible<T>)
      : CustomListBox<T>(elementName, label, Factory{}), ValueObservable<T>(),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), selectedItemIndex(selectedIdx),
        height(heightInItems) {
    setItems(newItems);
  }

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
  [[nodiscard]] std::optional<T> getSelectedItem() const {
    if (selectedItemIndex.has_value()) { return filteredItems[*selectedItemIndex]->first; }
    return std::nullopt;
  }

  /**
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemToSelect item to select
   */
  void setSelectedItem(const T &itemToSelect) requires(!std::same_as<T, std::string>) {
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
    }
  }

  void setSelectedItemByIndex(std::size_t index) {
    assert(index < items.size());
    if (index != selectedItemIndex) {
      if (selectedItemIndex.has_value()) { filteredItems[*selectedItemIndex]->setValue(false); }
      selectedItemIndex = index;
      filteredItems[*selectedItemIndex]->setValue(true);
      ValueObservable<T>::setValueInner(filteredItems[index]->first);
      ValueObservable<T>::notifyValueChanged();
    }
  }

 protected:
  void renderImpl() override {
    if (ImGui::BeginListBox(getName().c_str(), getSize().asImVec())) {
      std::ranges::for_each(items | ranges::views::enumerate, [this](auto &itemIdx) {
        const auto &[idx, item] = itemIdx;
        item.second->render();
        if (item->getValue()) {
          setSelectedItemByIndex(idx);
        }
      });
      ImGui::EndListBox();
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
  std::optional<int> selectedItemIndex = std::nullopt;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_LISTBOX_H
