//
// Created by xflajs00 on 29.03.2022.
//

#include "CheckboxPin.h"
#include <pf_imgui/unique_id.h>

namespace pf::ui::ig::bp {

BPCheckboxPin::BPCheckboxPin(const std::string &name, const std::string &label, Color color, const Width &width)
    : InteractablePin(name, label, color, width, Checkbox::Config{.name = uniqueId(), .label = label}) {}

std::unique_ptr<BPCheckboxPin> BPCheckboxPin::ConstructFromToml(const toml::table &src) {
  auto result = std::make_unique<BPCheckboxPin>("", "", Color::White, Width{0.f});
  result->setFromToml(src);
  return std::move(result);
}

}  // namespace pf::ui::ig::bp