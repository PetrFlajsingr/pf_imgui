//
// Created by petr on 10/31/20.
//

#include "ComboBox.h"
#include <algorithm>
#include <imgui.h>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <toml++/toml_node_view.h>
#include <utility>

namespace pf::ui::ig {

ComboBox::ComboBox(const std::string &elementName, const std::string &label, std::string previewValue,
                   std::vector<std::string> items, Persistent persistent)
    : Element(elementName), ItemElement(elementName),
      Labellable(label), ValueObservableElement<std::string_view>(elementName, ""),
      SavableElement(elementName, persistent), items(std::move(items)), previewValue(std::move(previewValue)) {}

void ComboBox::renderImpl() {
  using namespace ranges;
  if (ImGui::BeginCombo(getLabel().c_str(),
                        selectedItemIndex.has_value() ? items[*selectedItemIndex].c_str() : previewValue.c_str())) {
    auto cStrItems = items | views::transform([](const auto &str) { return str.c_str(); });
    std::ranges::for_each(cStrItems | views::enumerate
                              | views::filter([this](auto idxPtr) { return filter(idxPtr.second); }),
                          [&](auto idxPtr) {
                            const auto [idx, ptr] = idxPtr;
                            auto isSelected = selectedItemIndex.has_value() && *selectedItemIndex == idx;
                            ImGui::Selectable(ptr, &isSelected);
                            if (isSelected) {
                              if (!selectedItemIndex.has_value() || *selectedItemIndex != idx) {
                                setValue(items[idx]);
                                notifyValueChanged();
                              }
                              selectedItemIndex = idx;
                            }
                          });
    ImGui::EndCombo();
  }
}
std::optional<std::string_view> ComboBox::getSelectedItem() {
  if (!selectedItemIndex.has_value()) { return std::nullopt; }
  return items[*selectedItemIndex];
}
void ComboBox::removeItem(const std::string &item) {
  if (const auto iter = std::ranges::find(items, item); iter != items.end()) {
    items.erase(iter);
    selectedItemIndex = std::nullopt;
  }
}
void ComboBox::addItem(const std::string &item) { items.emplace_back(item); }

void ComboBox::unserialize_impl(const toml::table &src) {
  if (src.contains("selected")) {
    const auto idx = **src["selected"].as_integer();
    if (static_cast<std::size_t>(idx) < items.size()) {
      selectedItemIndex = idx;
      setValueAndNotifyIfChanged(items[idx]);
    }
  }
}

toml::table ComboBox::serialize_impl() {
  auto result = toml::table{};
  if (selectedItemIndex.has_value()) { result.insert_or_assign("selected", *selectedItemIndex); }
  return result;
}

const std::vector<std::string> &ComboBox::getItems() const { return items; }

void ComboBox::setItems(std::vector<std::string> newItems) {
  items = std::move(newItems);
  selectedItemIndex = std::nullopt;
}
void ComboBox::clearFilter() {
  filter = [](auto) { return true; };
}

}// namespace pf::ui::ig