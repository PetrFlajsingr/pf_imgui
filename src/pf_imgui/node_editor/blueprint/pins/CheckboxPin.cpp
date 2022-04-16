//
// Created by xflajs00 on 29.03.2022.
//

#include "CheckboxPin.h"
#include <pf_imgui/unique_id.h>

namespace pf::ui::ig::bp {

CheckboxPin::CheckboxPin(const std::string &name, const std::string &label, Color color, const Width &width)
    : InteractablePin(name, label, color, width, Checkbox::Config{.name = uniqueId(), .label = label}) {}

std::unique_ptr<CheckboxPin> CheckboxPin::ConstructFromToml(ig::Node *parent, const toml::table &src) {
  auto result = std::make_unique<CheckboxPin>("", "", Color::White, Width{0.f});
  result->parent = parent;
  result->setFromToml(src);
  return result;
}

}  // namespace pf::ui::ig::bp