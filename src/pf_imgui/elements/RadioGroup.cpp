//
// Created by petr on 10/31/20.
//

#include "RadioGroup.h"
#include <imgui.h>
#include <range/v3/view/enumerate.hpp>
#include <toml++/toml_node_view.h>
#include <utility>

namespace pf::ui::ig {

RadioGroup::RadioGroup(const std::string &elementName, const std::string &label, std::vector<RadioButton> buttons,
                       const std::optional<std::size_t> &selectedButtonIndex, Persistent persistent)
    : Element(elementName), Labellable(label), ValueObservable<std::string_view>(""), Savable(persistent),
      buttons(std::move(buttons)), selectedButtonIndex(selectedButtonIndex) {}

void RadioGroup::renderImpl() {
  ImGui::Text("%s:", getLabel().c_str());
  std::ranges::for_each(buttons, [](auto &button) { button.renderImpl(); });
  std::optional<std::size_t> newSelection = std::nullopt;
  std::ranges::for_each(buttons | ranges::views::enumerate, [&](auto idxBtn) {
    auto &[idx, btn] = idxBtn;
    if (btn.isSelected()) {
      if (!selectedButtonIndex.has_value() || idx != *selectedButtonIndex) { newSelection = idx; }
    }
  });
  if (newSelection.has_value()) {
    auto &selectedButton = buttons[*newSelection];
    std::ranges::for_each(buttons, [&](auto &button) {
      if (&button != &selectedButton) { button.setValueAndNotifyIfChanged(false); }
    });
    setValueInner(selectedButton.getLabel());
    selectedButtonIndex = newSelection;
    notifyValueChanged();
  }
}

RadioButton &RadioGroup::addButton(const std::string &elementName, const std::string &caption, bool value) {
  return buttons.emplace_back(elementName, caption, value);
}

void RadioGroup::unserialize_impl(const toml::table &src) {
  if (src.contains("selected")) {
    const auto idx = *src["selected"].value<int>();
    if (static_cast<std::size_t>(idx) < buttons.size()) {
      selectedButtonIndex = idx;
      auto &selectedButton = buttons[idx];
      selectedButton.setValueInner(true);
      setValueAndNotifyIfChanged(buttons[idx].getLabel());
      std::ranges::for_each(buttons, [&](auto &button) {
        if (&button != &selectedButton) { button.setValueInner(false); }
      });
    }
  }
}

toml::table RadioGroup::serialize_impl() {
  auto result = toml::table{};
  if (selectedButtonIndex.has_value()) { result.insert_or_assign("selected", static_cast<int>(*selectedButtonIndex)); }
  return result;
}

}// namespace pf::ui::ig