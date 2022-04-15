//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_CHECKBOXPIN_H
#define PF_IMGUI_BLUEPRINT_CHECKBOXPIN_H

#include "InteractablePin.h"
#include <pf_imgui/elements/Checkbox.h>
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {

class BPCheckboxPin : public InteractablePin<Checkbox> {
 public:
  PF_IMGUI_BLUEPRINT_OVERRIDE_GETTYPEID(BPCheckboxPin)
  BPCheckboxPin(const std::string &name, const std::string &label, Color color, const Width &width);
};

}  // namespace pf::ui::ig::bp
#endif  //PF_IMGUI_BLUEPRINT_CHECKBOXPIN_H
