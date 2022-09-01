//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_CHECKBOXPIN_H
#define PF_IMGUI_BLUEPRINT_CHECKBOXPIN_H

#include "InteractablePin.h"
#include <pf_imgui/elements/Checkbox.h>
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {

class CheckboxPin : public InteractablePin<Checkbox> {
 public:
  PF_IMGUI_BLUEPRINT_PIN_ID(CheckboxPin)
  CheckboxPin(const std::string &elementName, const std::string &labelText, Color pinColor, const Width &width);

  [[nodiscard]] static std::unique_ptr<CheckboxPin> ConstructFromToml(ig::Node *parent, const toml::table &src);
};

}  // namespace pf::ui::ig::bp
#endif  //PF_IMGUI_BLUEPRINT_CHECKBOXPIN_H
