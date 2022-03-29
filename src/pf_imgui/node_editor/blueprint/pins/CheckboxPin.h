//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_CHECKBOXPIN_H
#define IMGUI_EXPERIMENTS_CHECKBOXPIN_H

#include "InteractablePin.h"
#include <pf_imgui/elements/Checkbox.h>

namespace pf::ui::ig::bp {

class BPCheckboxPin : public InteractablePin<Checkbox> {
 public:
  BPCheckboxPin(const std::string &name, const std::string &label, const ImVec4 &color, const Width &width);
};

}  // namespace pf::ui::ig::bp
#endif  //IMGUI_EXPERIMENTS_CHECKBOXPIN_H
