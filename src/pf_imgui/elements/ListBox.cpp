//
// Created by petr on 11/1/20.
//

#include "ListBox.h"
#include <imgui.h>
#include <range/v3/core.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/transform.hpp>
#include <utility>

namespace pf::ui::ig {

ListBox::ListBox(const std::string &elementName, const std::string &caption, std::vector<std::string> items_,
                 int selectedIdx, int heightInItems)
    : Element(elementName), ItemElement(elementName), LabeledElement(elementName, caption),
      ValueObservableElement(elementName, ""), items(std::move(items_)), currentItemIdx(selectedIdx),
      height(heightInItems) {}

void ListBox::renderImpl() {
  const auto cStrItems =
      items | ranges::views::transform([](const auto &str) { return str.c_str(); }) | ranges::to_vector;
  const auto oldIdx = currentItemIdx;
  ImGui::ListBox(getLabel().c_str(), &currentItemIdx, cStrItems.data(), cStrItems.size(), height);
  if (currentItemIdx != oldIdx) {
    setValue(items[currentItemIdx]);
    notifyValueChanged();
  }
}

void ListBox::addItem(std::string item) { items.emplace_back(std::move(item)); }

std::string_view ListBox::getSelectedItem() const { return items[currentItemIdx]; }

void ListBox::selectItem(std::string_view itemToSelect) {
  for (const auto &[idx, item] : ranges::views::enumerate(items)) {
    if (item == itemToSelect) {
      currentItemIdx = idx;
      break;
    }
  }
}

}// namespace pf::ui::ig