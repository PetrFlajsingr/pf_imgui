//
// Created by xflajs00 on 29.03.2022.
//

#include "CheckboxPin.h"
#include <pf_imgui/unique_id.h>

namespace pf::ui::ig::bp {

CheckboxPin::CheckboxPin(std::string_view elementName, std::string_view labelText, Color pinColor, Width width)
    : InteractablePin(elementName, labelText, pinColor, width,
                      Checkbox::Config{.name = uniqueId(), .label = labelText}) {}

std::unique_ptr<CheckboxPin> CheckboxPin::ConstructFromToml(ig::Node *parent, const toml::table &src) {
  auto result = std::make_unique<CheckboxPin>("", "", Color::White, Width{0.f});
  result->parent = parent;
  result->setFromToml(src);
  return result;
}

}  // namespace pf::ui::ig::bp