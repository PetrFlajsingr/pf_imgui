//
// Created by petr on 10/31/20.
//

#include "RadioGroup.h"
#include <imgui.h>
#include <memory>
#include <range/v3/view/enumerate.hpp>
#include <utility>

namespace pf::ui::ig {

RadioGroup::RadioGroup(const std::string &elementName, std::unique_ptr<Resource<std::string>> label,
                       std::vector<std::unique_ptr<RadioButton>> buttons,
                       const std::optional<std::size_t> &selectedButtonIndex, Persistent persistent)
    : Element(elementName), Labellable(std::move(label)), ValueObservable<std::string_view>(""), Savable(persistent),
      buttons(std::move(buttons)), selectedButtonIndex(selectedButtonIndex) {}

void RadioGroup::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  ImGui::Text("%s:", getLabel().get().c_str());
  std::ranges::for_each(buttons, [](auto &button) { button->renderImpl(); });
  std::optional<std::size_t> newSelection = std::nullopt;
  std::ranges::for_each(buttons | ranges::views::enumerate, [&](auto idxBtn) {
    const auto &[idx, btn] = idxBtn;
    if (btn->isSelected()) {
      if (!selectedButtonIndex.has_value() || idx != *selectedButtonIndex) { newSelection = idx; }
    }
  });
  if (newSelection.has_value()) {
    auto &selectedButton = buttons[*newSelection];
    std::ranges::for_each(buttons, [&](auto &button) {
      if (&button != &selectedButton) { button->setValueAndNotifyIfChanged(false); }
    });
    setValueInner(selectedButton->getLabel().get());
    selectedButtonIndex = newSelection;
    notifyValueChanged();
  }
}

RadioButton &RadioGroup::addButton(const std::string &elementName, std::unique_ptr<Resource<std::string>> label, bool initValue) {
  return *buttons.emplace_back(std::make_unique<RadioButton>(elementName, std::move(label), initValue));
}

void RadioGroup::unserialize_impl(const toml::table &src) {
  if (auto newValIter = src.find("selected"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<int>(); newVal.has_value()) {
      const auto idx = newVal.value();
      if (static_cast<std::size_t>(idx) >= buttons.size()) { return; }
      selectedButtonIndex = idx;
      auto &selectedButton = buttons[idx];
      selectedButton->setValueInner(true);
      setValueAndNotifyIfChanged(buttons[idx]->getLabel().get());
      std::ranges::for_each(buttons, [&](auto &button) {
        if (&button != &selectedButton) { button->setValueInner(false); }
      });
    }
  }

  if (src.contains("selected")) {
    const auto idx = *src["selected"].value<int>();
    if (static_cast<std::size_t>(idx) < buttons.size()) {
      selectedButtonIndex = idx;
      auto &selectedButton = buttons[idx];
      selectedButton->setValueInner(true);
      setValueAndNotifyIfChanged(buttons[idx]->getLabel().get());
      std::ranges::for_each(buttons, [&](auto &button) {
        if (&button != &selectedButton) { button->setValueInner(false); }
      });
    }
  }
}

toml::table RadioGroup::serialize_impl() const {
  auto result = toml::table{};
  if (selectedButtonIndex.has_value()) { result.insert_or_assign("selected", static_cast<int>(*selectedButtonIndex)); }
  return result;
}

}  // namespace pf::ui::ig
