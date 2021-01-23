//
// Created by petr on 10/31/20.
//

#include "RadioGroup.h"
#include <range/v3/view/enumerate.hpp>
#include <toml++/toml_node_view.h>
#include <utility>
#include <imgui.h>

namespace pf::ui::ig {

RadioGroup::RadioGroup(const std::string &elementName, const std::string &label, std::vector<RadioButton> buttons,
                       const std::optional<std::size_t> &selectedButtonIndex, Persistent persistent)
    : Element(elementName), Labellable(label), ValueObservableElement<std::string_view>(elementName, ""),
      SavableElement(elementName, persistent), buttons(std::move(buttons)), selectedButtonIndex(selectedButtonIndex) {}

void RadioGroup::renderImpl() {
  ImGui::Separator();
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
      if (&button != &selectedButton) { button.setValue(false); }
    });
    selectedButton.notifyValueChanged();
    setValue(selectedButton.getLabel());
    selectedButtonIndex = newSelection;
    notifyValueChanged();
  }
  ImGui::Separator();
}

void RadioGroup::addButton(const std::string &elementName, const std::string &caption, bool value) {
  buttons.emplace_back(elementName, caption, value);
}

void RadioGroup::unserialize_impl(const toml::table &src) {
  if (src.contains("selected")) {
    const auto idx = **src["selected"].as_integer();
    if (static_cast<std::size_t>(idx) < buttons.size()) {
      selectedButtonIndex = idx;
      auto &selectedButton = buttons[idx];
      selectedButton.setValue(true);
      setValueAndNotifyIfChanged(buttons[idx].getLabel());
      std::ranges::for_each(buttons, [&](auto &button) {
        if (&button != &selectedButton) { button.setValue(false); }
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