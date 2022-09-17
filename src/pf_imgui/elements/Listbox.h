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
#include <pf_common/Explicit.h>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/CustomListbox.h>
#include <pf_imgui/elements/Selectable.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/unique_id.h>
#include <range/v3/view/addressof.hpp>
#include <range/v3/view/enumerate.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

namespace details {
template<ToStringConvertible T>
struct PF_IMGUI_EXPORT ListboxRowFactory {
  static inline std::size_t idCounter{};
  const std::string idStart = uniqueId();
  std::unique_ptr<Selectable> operator()(const T &item) {
    return std::make_unique<Selectable>(idStart + std::to_string(idCounter++), toString(item));
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
                                public ValueContainer<std::optional<T>>,
                                public Savable,
                                public DragSource<T>,
                                public DropTarget<T> {
 protected:
  PF_IMGUI_PROPERTY_OWNER_ALIASES(U)
  using CustomListboxBase = CustomListbox<T, Selectable>;
  using CustomListboxBase::filter;
  using CustomListboxBase::filteredItems;
  using CustomListboxBase::items;

 public:
  using Factory = details::ListboxRowFactory<T>;
  using CustomListboxBase::addItem;
  using CustomListboxBase::addItems;
  using CustomListboxBase::clearFilter;
  using CustomListboxBase::getItems;
  using CustomListboxBase::getName;
  using CustomListboxBase::removeItem;
  using CustomListboxBase::removeItemIf;
  using CustomListboxBase::setFilter;
  using CustomListboxBase::setItems;
  /**
   * @brief Struct for construction of Listbox.
   */
  struct Config {
    using Parent = Listbox;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the Listbox */
    Size size = Size::Auto();         /*!< Size of the element */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct Listbox
   * @param config construction args @see Listbox::Config
   */
  explicit Listbox(Config &&config)
    requires(std::is_default_constructible_v<T> && std::copy_constructible<T>);
  /**
   * Construct Listbox.
   * @param elementName ID of the element
   * @param labelText text rendered at the top of the list
   * @param s size of the element
   * @param persistent enable/disable state saving to disk
   */
  Listbox(std::string_view elementName, std::string_view labelText, Size s = Size::Auto(),
          Persistent persistent = Persistent::No)
    requires(std::is_default_constructible_v<T> && std::copy_constructible<T>);

  /**
   * Add item to the end of the list.
   * @param item item to be added
   * @param selected when selected the item is activated in the same way as if the user clicked on it
   * and observers are notified
   */
  Selectable &addItem(const T &item, Selected selected);

  /**
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemAsString string representation of item to select
   */
  void setSelectedItemAsString(const std::string &itemAsString);

  /**
   * Select item by its index.
   * @param index index to select
   */
  void setSelectedItemByIndex(std::size_t index);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  Property<std::optional<T>> selectedItem;

  [[nodiscard]] const std::optional<T> &getValue() const override;
  void setValue(const std::optional<T> &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const std::optional<T> &)> listener) override;

  void renderImpl() override;

  void refilterItems() override;

 private:
  std::optional<std::size_t> selectedItemIndex = std::nullopt;
};

template<ToStringConvertible T>
Listbox<T>::Listbox(Listbox::Config &&config)
  requires(std::is_default_constructible_v<T> && std::copy_constructible<T>)
: Listbox{config.name, config.label, config.size, config.persistent ? Persistent::Yes : Persistent::No} {}

template<ToStringConvertible T>
Listbox<T>::Listbox(std::string_view elementName, std::string_view labelText, Size s, Persistent persistent)
  requires(std::is_default_constructible_v<T> && std::copy_constructible<T>)
: CustomListboxBase(elementName, labelText, Factory{}, s),
  Savable(persistent), DragSource<T>(false), DropTarget<T>(false) {

  // TODO: clean this up
  selectedItem.addListener([this](const auto &itemToSelect) {
    if (!itemToSelect.has_value()) { selectedItemIndex = std::nullopt; }
    if constexpr (std::equality_comparable<T>) {
      if (const auto iter =
              std::ranges::find_if(CustomListboxBase::filteredItems,
                                   [&itemToSelect](const auto &item) { return item->first == *itemToSelect; });
          iter != CustomListboxBase::filteredItems.end()) {
        const auto index = std::ranges::distance(CustomListboxBase::filteredItems.begin(), iter);
        setSelectedItemByIndex(index);
      }
    } else {
      const auto itemAsString = toString(*itemToSelect);
      setSelectedItemAsString(itemAsString);
    }
  });
}

template<ToStringConvertible T>
Selectable &Listbox<T>::addItem(const T &item, Selected selected) {
  auto &result = addItem(item);
  if (selected == Selected::Yes) { *selectedItem.modify() = item; }
  return result;
}

template<ToStringConvertible T>
void Listbox<T>::setSelectedItemAsString(const std::string &itemAsString) {
  if (const auto iter = std::ranges::find_if(
          filteredItems, [itemAsString](const auto &item) { return item->second->label.get() == itemAsString; });
      iter != filteredItems.end()) {
    const auto index = std::ranges::distance(filteredItems.begin(), iter);
    setSelectedItemByIndex(index);
  }
}

template<ToStringConvertible T>
void Listbox<T>::setSelectedItemByIndex(std::size_t index) {
  assert(index < items.size());
  if (index != selectedItemIndex) {
    if (selectedItemIndex.has_value()) { filteredItems[*selectedItemIndex]->second->setValue(false); }
    selectedItemIndex = index;
    filteredItems[*selectedItemIndex]->second->setValue(true);
    *selectedItem.modify() = filteredItems[index]->first;
  } else {
    *selectedItem.modify() = std::nullopt;
  }
}

template<ToStringConvertible T>
toml::table Listbox<T>::toToml() const {
  auto result = toml::table{};
  if (selectedItemIndex.has_value()) {
    const auto currentItem = filteredItems[*selectedItemIndex];
    result.insert_or_assign("selected", currentItem->second->label->get());
  }
  return result;
}

template<ToStringConvertible T>
void Listbox<T>::setFromToml(const toml::table &src) {
  if (auto selectedValIter = src.find("selected"); selectedValIter != src.end()) {
    if (auto selectedVal = selectedValIter->second.value<std::string>(); selectedVal.has_value()) {
      *selectedItem.modify() = selectedVal.value();
    }
  }
}

template<ToStringConvertible T>
void Listbox<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = this->color.applyScoped();
  [[maybe_unused]] auto styleScoped = this->style.applyScoped();
  [[maybe_unused]] auto fontScoped = this->font.applyScopedIfNotDefault();
  if (ImGui::BeginListBox(this->label->get().c_str(), static_cast<ImVec2>(*this->size))) {
    RAII end{ImGui::EndListBox};
    std::ranges::for_each(filteredItems | ranges::views::enumerate, [this](const auto &itemIdx) {
      const auto &[idx, item] = itemIdx;
      item->second->render();
      if (item->second->getValue()) { setSelectedItemByIndex(idx); }
    });
  }
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) { addItem(*drop); }
  if (selectedItemIndex.has_value()) { DragSource<T>::drag(filteredItems[*selectedItemIndex]->first); }
}

template<ToStringConvertible T>
void Listbox<T>::refilterItems() {
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

template<ToStringConvertible T>
const std::optional<T> &Listbox<T>::getValue() const {
  return *selectedItem;
}

template<ToStringConvertible T>
void Listbox<T>::setValue(const std::optional<T> &newValue) {
  *selectedItem.modify() = newValue;
}

template<ToStringConvertible T>
Subscription Listbox<T>::addValueListenerImpl(std::function<void(const std::optional<T> &)> listener) {
  return selectedItem.addListener(std::move(listener));
}

extern template class Listbox<std::string>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_LISTBOX_H
