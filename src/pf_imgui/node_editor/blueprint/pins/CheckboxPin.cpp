//
// Created by xflajs00 on 29.03.2022.
//

#include "CheckboxPin.h"
#include <pf_imgui/unique_id.h>

namespace pf::ui::ig::bp {

BPCheckboxPin::BPCheckboxPin(const std::string &name, const std::string &label, const ImVec4 &color, const Width &width)
    : InteractablePin(name, label, color, width, Checkbox::Config{.name = uniqueId(), .label = label}) {}

}  // namespace pf::ui::ig::bp