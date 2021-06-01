//
// Created by petr on 6/1/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H

#include <algorithm>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <vector>

namespace pf::ui::ig {

template<typename F, typename T, typename R>
concept CustomListboxRowFactory = std::is_invocable_r_v<std::unique_ptr<R>, F, T>;

template<typename T, std::derived_from<Renderable> R>
class PF_IMGUI_EXPORT CustomListbox : public ItemElement, public Labellable, public Resizable {
 public:
  CustomListbox(const std::string &elementName, const std::string &label,
                CustomListboxRowFactory<T, R> auto &&rowFactory, Size s = Size::Auto())
      : ItemElement(elementName), Labellable(label), Resizable(s),
        factory(std::forward<decltype(rowFactory)>(rowFactory)) {}

  R &addItem(const T &item) {
    auto &result = *items.template emplace_back(item, factory(item))->second.get();
    refilterItems();
    return result;
  }

  std::vector<std::reference_wrapper<R>> addItems(std::ranges::range auto &&newItems) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(items)>, T>) {
    auto result = std::vector<std::reference_wrapper<R>>{};
    std::ranges::for_each(newItems, [this](const auto &newItem) {
      result.template emplace_back(*items.template emplace_back(item, factory(item))->second.get());
    });
    refilterItems();
    return result;
  }

  void setItems(std::ranges::range auto &&newItems) requires(
      std::convertible_to<std::ranges::range_value_t<decltype(data)>, T>) {
    items.clear();
    std::ranges::copy(newItems, std::back_inserter(items));
    refilterItems();
  }

  void removeItem(const T &itemToRemove) requires(std::equality_comparable<T>) {
    std::erase_if(items, [this](const auto &item) { return item.first == itemToRemove; });
    refilterItems();
  }
  void removeItemIf(std::predicate<const T &> auto &&predicate) {
    std::erase_if(items,
                  [p = std::forward<decltype(predicate)>(predicate)](const auto &item) { return p(item.first); });
    refilterItems();
  }

  void setFilter(std::predicate<const T &> auto filterFnc) {
    filter = filterFnc;
    refilterItems();
  }

  void clearFilter() {
    filter = [](auto) { return true; };
    refilterItems();
  }

  [[nodiscard]] auto getItems() {
    return items | std::views::transform([](auto &item) -> T & { return item.first; });
  }
  [[nodiscard]] auto getItems() const {
    return items | std::views::transform([](auto &item) -> const T & { return item.first; });
  }

 protected:
  void refilterItems() {
    filteredItems = items | std::views::filter([this](auto &item) { return filter(item.first); })
        | std::views::transform([](auto &item) { return item.second.get(); }) | ranges::to_vector;
  }

  void renderImpl() override {
    if (ImGui::BeginListBox(getName().c_str(), getSize().asImVec())) {
      std::ranges::for_each(items, [](auto &item) { item.second->render(); });
      ImGui::EndListBox();
    }
  }

  std::vector<std::pair<T, std::unique_ptr<R>>> items;
  std::vector<R *> filteredItems;
  std::std::function<std::unique_ptr<R>(const T &)> > factory;
  std::function<bool(const T &)> filter = [](const auto &) { return true; };
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H
