//
// Created by petr on 6/4/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMCOMBOBOX_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMCOMBOBOX_H

#include <imgui.h>
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>

namespace pf::ui::ig {
/**
 * Factory of Combobox's rows.
 * @tparam F factory
 * @tparam T input/storage type
 * @tparam R resulting renderable for row
 */
template<typename F, typename T, typename R>
concept CustomComboboxRowFactory = std::is_invocable_r_v<std::unique_ptr<R>, F, T>;

enum class ComboBoxCount { Items4 = 1 << 1, Items8 = 1 << 2, Items20 = 1 << 3, ItemsAll = 1 << 4 };

/**
 * @brief Combobox where rows can be whatever is provided by the user.
 *
 * Rows are created using a factory from stored value.
 * @tparam T type stored in each row
 * @tparam R type stored in each row
 */
template<typename T, std::derived_from<Renderable> R>
class PF_IMGUI_EXPORT CustomCombobox : public ItemElement, public Labellable {
 public:
  CustomCombobox(const std::string &elementName, const std::string &label,
                 CustomComboboxRowFactory<T, R> auto &&rowFactory, std::string prevValue = "",
                 ComboBoxCount showItemCount = ComboBoxCount::Items8)
      : ItemElement(elementName), Labellable(label), factory(std::forward<decltype(rowFactory)>(rowFactory)),
        flags(static_cast<ImGuiComboFlags_>(showItemCount)), previewValue(std::move(prevValue)) {
    if (previewValue.empty()) { flags |= ImGuiComboFlags_::ImGuiComboFlags_NoPreview; }
  }

  /**
   * Add item to the end of the list.
   * @param item item to be added
   * @return reference to the Renderable representing the newly created row
   */
  R &addItem(const T &item) {
    auto &result = *items.template emplace_back(item, factory(item)).second.get();
    refilterItems();
    return result;
  }

  /**
   * Add multiple item to the end of the list.
   * @param newItems items to be added
   * @return references to the newly created rows
   */
  std::vector<std::reference_wrapper<R>> addItems(std::ranges::range auto &&newItems) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>) {
    auto result = std::vector<std::reference_wrapper<R>>{};
    std::ranges::for_each(newItems, [this, &result](const auto &newItem) {
      result.template emplace_back(*items.template emplace_back(newItem, factory(newItem)).second.get());
    });
    refilterItems();
    return result;
  }

  /**
   * Clear the listbox and set new items.
   * @param newItems items to set
   * @return references to the newly created rows
   */
  std::vector<std::reference_wrapper<R>> setItems(std::ranges::range auto &&newItems) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(newItems)>, T>) {
    items.clear();
    return addItems(std::forward<decltype(newItems)>(newItems));
  }

  /**
   * Remove an item.
   * @param itemToRemove item to remove
   */
  void removeItem(const T &itemToRemove) requires(std::equality_comparable<T>) {
    std::erase_if(items, [this, &itemToRemove](const auto &item) { return item.first == itemToRemove; });
    refilterItems();
  }
  /**
   * Remove all items for which predicate returns true.
   * @param predicate predicate for item removal
   */
  void removeItemIf(std::predicate<const T &> auto &&predicate) {
    std::erase_if(items,
                  [p = std::forward<decltype(predicate)>(predicate)](const auto &item) { return p(item.first); });
    refilterItems();
  }

  /**
   * Set filter by which shown items are filtered.
   * @param filterFnc
   */
  void setFilter(std::predicate<const T &> auto filterFnc) {
    filter = filterFnc;
    refilterItems();
  }
  /**
   * Remove filter.
   */
  void clearFilter() {
    filter = [](auto) { return true; };
    refilterItems();
  }
  /**
   * Get reference to all items.
   * @return references to all items
   */
  [[nodiscard]] auto getItems() {
    return items | std::views::transform([](auto &item) -> T & { return item.first; });
  }
  /**
   * Get reference to all items.
   * @return references to all items
   */
  [[nodiscard]] auto getItems() const {
    return items | std::views::transform([](auto &item) -> const T & { return item.first; });
  }

  void setPreviewValue(std::string value) {
    previewValue = std::move(value);
    if (previewValue.empty()) { flags |= ImGuiComboFlags_::ImGuiComboFlags_NoPreview; }
  }
  [[nodiscard]] const std::string &getPreviewValue() const { return previewValue; }

  /**
   * Get count of items shown when the element is unrolled.
   * @return count of items
   */
  [[nodiscard]] ComboBoxCount getShownItemCount() const {// FIXME
    return static_cast<ComboBoxCount>(*(flags & ImGuiComboFlags_::ImGuiComboFlags_HeightMask_));
  }
  /**
   * Set count of items shown when the element is unrolled.
   * @param shownItemCount count of items
   */
  void setShownItemCount(ComboBoxCount shownItemCount) {
    flags &= static_cast<ImGuiComboFlags_>(~ImGuiComboFlags_::ImGuiComboFlags_HeightMask_);
    flags |= static_cast<ImGuiComboFlags_>(shownItemCount);
  }

 protected:
  virtual void refilterItems() {
    filteredItems = items | std::views::filter([this](auto &item) { return filter(item.first); })
        | std::views::transform([](auto &item) { return &item; }) | ranges::to_vector;
  }

  void renderImpl() override {
    const char *previewPtr = previewValue.c_str();
    if (ImGui::BeginCombo(getLabel().c_str(), previewPtr, *flags)) {
      std::ranges::for_each(filteredItems, [](auto item) { item->second->render(); });
      ImGui::EndCombo();
    }
  }
  using Item = std::pair<T, std::unique_ptr<R>>;
  std::vector<Item> items;
  std::vector<Item *> filteredItems;
  std::function<std::unique_ptr<R>(const T &)> factory;
  std::function<bool(const T &)> filter = [](const auto &) { return true; };

  Flags<ImGuiComboFlags_> flags{};

 private:
  std::string previewValue{};
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMCOMBOBOX_H
