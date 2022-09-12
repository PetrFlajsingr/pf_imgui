/**
 * @file Combobox.h
 * @brief Selectable combobox.
 * @author Petr Flajšingr
 * @date 4.6.21
 */

#ifndef PF_IMGUI_ELEMENTS_COMBOBOX_H
#define PF_IMGUI_ELEMENTS_COMBOBOX_H

#include <memory>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/CustomCombobox.h>
#include <pf_imgui/elements/Selectable.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
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
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT Combobox : public CustomCombobox<T, Selectable>,
                                 public ValueContainer<std::optional<T>>,
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
    Explicit<std::string_view> name;                      /*!< Unique name of the element */
    Explicit<std::string_view> label;                     /*!< Text rendered next to the Combobox */
    Explicit<std::string> preview;                        /*!< Preview value shown when no item is selected */
    ComboBoxCount shownItemCount = ComboBoxCount::Items8; /*!< Amount of items shown when Combobox is open */
    bool persistent = false;                              /*!< Allow state saving to disk */
  };
  /**
   * Construct Combobox
   * @param config construction args @see Combobox::Config
   */
  explicit Combobox(Config &&config);
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
           Persistent persistent = Persistent::No)
    requires(std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>
             && std::is_default_constructible_v<T> && std::copy_constructible<T>);
  ;

  /**
   * Set selected item by its string value. If no such item is found the selection is cancelled.
   * @param itemAsString item to be selected
   */
  void setSelectedItemAsString(const std::string &itemAsString);
  /**
   * Select item by its index.
   * @param index index to select
   */
  void setSelectedItemByIndex(std::size_t index);
  /**
   * Cancel selection and show previewValue.
   */
  void cancelSelection();

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ObservableProperty<Combobox, std::optional<T>> selectedItem;

  [[nodiscard]] const std::optional<T> &getValue() const override;
  void setValue(const std::optional<T> &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const std::optional<T> &)> listener) override;

  void renderImpl() override;

 private:
  std::optional<unsigned int> selectedItemIndex = std::nullopt;
};

template<ToStringConvertible T>
Combobox<T>::Combobox(Combobox::Config &&config)
    : Combobox{std::string{config.name.value}, std::string{config.label.value},
               std::string{config.preview},    std::vector<T>{},
               config.shownItemCount,          config.persistent ? Persistent::Yes : Persistent::No} {}

template<ToStringConvertible T>
Combobox<T>::Combobox(const std::string &elementName, const std::string &label, const std::string &prevValue,
                      std::ranges::range auto &&newItems, ComboBoxCount showItemCount, Persistent persistent)
  requires(std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T> && std::is_default_constructible_v<T>
           && std::copy_constructible<T>)
: CustomComboboxBase(elementName, label, details::ComboboxRowFactory<T>{}, prevValue, showItemCount),
  Savable(persistent), DragSource<T>(false), selectedItem(std::nullopt) {
  addItems(std::forward<decltype(newItems)>(newItems));

  // TODO: clean this up
  selectedItem.addListener([this](const auto &itemToSelect) {
    if (!itemToSelect.has_value()) { selectedItemIndex = std::nullopt; }
    if constexpr (std::equality_comparable<T>) {
      if (const auto iter =
              std::ranges::find_if(items, [&itemToSelect](const auto &item) { return item.first == *itemToSelect; });
          iter != items.end()) {
        const auto index = std::distance(items.begin(), iter);
        setSelectedItemByIndex(index);
      }
    } else {
      const auto itemAsString = toString(*itemToSelect);
      setSelectedItemAsString(itemAsString);
    }
  });
}

template<ToStringConvertible T>
void Combobox<T>::setSelectedItemAsString(const std::string &itemAsString) {
  if (const auto iter = std::ranges::find_if(
          items, [itemAsString](const auto &item) { return item.second->label.get() == itemAsString; });
      iter != items.end()) {
    const auto index = std::ranges::distance(items.begin(), iter);
    setSelectedItemByIndex(index);
  } else {
    cancelSelection();
  }
}

template<ToStringConvertible T>
void Combobox<T>::setSelectedItemByIndex(std::size_t index) {
  assert(index < items.size());
  if (index != selectedItemIndex) {
    if (selectedItemIndex.has_value()) { filteredItems[*selectedItemIndex]->second->setValue(false); }
    selectedItemIndex = static_cast<int>(index);
    filteredItems[*selectedItemIndex]->second->setValue(true);
    *selectedItem.modify() = filteredItems[index]->first;
  }
}

template<ToStringConvertible T>
toml::table Combobox<T>::toToml() const {
  auto result = toml::table{};
  if (selectedItemIndex.has_value()) {
    const auto currentItem = filteredItems[*selectedItemIndex];
    result.insert_or_assign("selected", currentItem->second->label->get());
  }
  return result;
}

template<ToStringConvertible T>
void Combobox<T>::setFromToml(const toml::table &src) {
  if (auto selectedValIter = src.find("selected"); selectedValIter != src.end()) {
    if (auto selectedVal = selectedValIter->second.value<std::string>(); selectedVal.has_value()) {
      *selectedItem.modify() = selectedVal.value();
    }
  }
}

template<ToStringConvertible T>
void Combobox<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = this->color.applyScoped();
  [[maybe_unused]] auto styleScoped = this->style.applyScoped();
  [[maybe_unused]] auto fontScoped = this->font.applyScopedIfNotDefault();
  const char *previewPtr;
  if (selectedItemIndex.has_value()) {
    previewPtr = filteredItems[*selectedItemIndex]->second->label->get().c_str();
  } else {
    previewPtr = getPreviewValue().c_str();
  }
  if (ImGui::BeginCombo(this->label->get().c_str(), previewPtr, *flags)) {
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

template<ToStringConvertible T>
const std::optional<T> &Combobox<T>::getValue() const {
  return *selectedItem;
}

template<ToStringConvertible T>
Subscription Combobox<T>::addValueListenerImpl(std::function<void(const std::optional<T> &)> listener) {
  return selectedItem.addListener(std::move(listener));
}

template<ToStringConvertible T>
void Combobox<T>::setValue(const std::optional<T> &newValue) {
  *selectedItem.modify() = newValue;
}

template<ToStringConvertible T>
void Combobox<T>::cancelSelection() {
  selectedItemIndex = std::nullopt;
  *selectedItem.modify() = std::nullopt;
}

extern template class Combobox<std::string>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_COMBOBOX_H
