/**
 * @file CustomListBox.h
 * @brief Customizable listbox.
 * @author Petr Flajšingr
 * @date 1.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H

#include <algorithm>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <vector>

namespace pf::ui::ig {

/**
 * Factory of Listbox's rows.
 * @tparam F factory
 * @tparam T input/storage type
 * @tparam R resulting renderable for row
 */
template<typename F, typename T, typename R>
concept CustomListboxRowFactory = std::is_invocable_r_v<std::unique_ptr<R>, F, T>;

/**
 * @brief Listbox where rows can be whatever is provided by the user.
 *
 * Rows are created using a factory from stored value.
 * @tparam T type stored in each row
 * @tparam R type stored in each row
 */
template<typename T, std::derived_from<Renderable> R>
class PF_IMGUI_EXPORT CustomListbox : public ItemElement, public Labellable, public Resizable {
 public:
  /**
   * Create CustomListbox
   * @param elementName id of the element
   * @param label text rendered next to the element
   * @param rowFactory factory for row creation
   * @param s size of the element
   */
  CustomListbox(const std::string &elementName, const std::string &label,
                CustomListboxRowFactory<T, R> auto &&rowFactory, Size s = Size::Auto())
      : ItemElement(elementName), Labellable(label), Resizable(s),
        factory(std::forward<decltype(rowFactory)>(rowFactory)) {}

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

 protected:
  virtual void refilterItems() {
    filteredItems = items | std::views::filter([this](auto &item) { return filter(item.first); })
        | std::views::transform([](auto &item) { return &item; }) | ranges::to_vector;
  }

  void renderImpl() override {
    if (ImGui::BeginListBox(getLabel().c_str(), getSize().asImVec())) {
      std::ranges::for_each(filteredItems, [](auto &item) { item->second->render(); });
      ImGui::EndListBox();
    }
  }

  using Item = std::pair<T, std::unique_ptr<R>>;
  std::vector<Item> items;
  std::vector<Item *> filteredItems;
  std::function<std::unique_ptr<R>(const T &)> factory;
  std::function<bool(const T &)> filter = [](const auto &) { return true; };
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H