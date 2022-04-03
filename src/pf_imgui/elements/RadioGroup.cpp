//
// Created by petr on 10/31/20.
//

#include "RadioGroup.h"
#include <imgui.h>
#include <memory>
#include <range/v3/view/enumerate.hpp>
#include <utility>

namespace pf::ui::ig {

RadioGroup::RadioGroup(RadioGroup::Config &&config)
    : ValueObservable(nullptr), Savable(config.persistent), groupName(std::string{config.groupName}),
      buttons(std::move(config.buttons)) {
  bool wasAnySelected = false;
  std::ranges::for_each(buttons, [this, &wasAnySelected](RadioButton *btn) {
    if (wasAnySelected) {
      btn->setValue(false);
    } else {
      wasAnySelected = btn->getValue();
    }
    addDestroyListener(btn);
  });
}

RadioGroup::RadioGroup(const std::string &groupName, std::vector<RadioButton *> buttons, Persistent persistent)
    : ValueObservable(nullptr), Savable(persistent), groupName(std::string{groupName}), buttons(std::move(buttons)) {
  std::ranges::for_each(buttons, [this](RadioButton *btn) { addDestroyListener(btn); });
}

void RadioGroup::frame() {
  RadioButton *newSelection = nullptr;
  std::ranges::for_each(buttons | ranges::views::enumerate, [&](const auto &idxBtn) {
    const auto &[idx, btn] = idxBtn;
    if (btn->isSelected()) {
      if (getValue() != btn) { newSelection = btn; }
    }
  });
  if (newSelection != nullptr) {
    std::ranges::for_each(buttons, [&](auto &button) {
      if (button != newSelection) { button->setValueAndNotifyIfChanged(false); }
    });
    setValueInner(newSelection);
    notifyValueChanged();
  }
}

void RadioGroup::addButton(RadioButton &button) {
  buttons.emplace_back(&button);
  addDestroyListener(buttons.back());

  if (getValue() != nullptr) {
    button.setValue(false);
  } else if (button.getValue()) {
    setValueAndNotifyIfChanged(buttons.back());
  }
}

const std::string &RadioGroup::getGroupName() const { return groupName; }

void RadioGroup::unserialize_impl(const toml::table &src) {
  if (auto newValIter = src.find("selected"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<std::string>(); newVal.has_value()) {
      const auto name = newVal.value();
      const auto btnIter = std::ranges::find(buttons, name, &RadioButton::getName);
      if (btnIter != buttons.end()) { return; }
      auto selectedButton = *btnIter;
      selectedButton->setValueInner(true);
      setValueAndNotifyIfChanged(selectedButton);
      std::ranges::for_each(buttons, [&](auto button) {
        if (button != selectedButton) { button->setValueInner(false); }
      });
    }
  }
}

toml::table RadioGroup::serialize_impl() const {
  auto result = toml::table{};
  if (getValue() != nullptr) { result.insert_or_assign("selected", getValue()->getName()); }
  return result;
}

void RadioGroup::addDestroyListener(RadioButton *button) {
  button->addDestroyListener([button, this] { buttons.erase(std::remove(buttons.begin(), buttons.end(), button)); });
}

}  // namespace pf::ui::ig
