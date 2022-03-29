//
// Created by xflajs00 on 29.03.2022.
//

#include "StringInputPin.h"

namespace pf::ui::ig::bp {

StringInputPin::StringInputPin(const std::string &name, const std::string &label, const ImVec4 &color,
                               StringInputPin::InputConfig &&config)
    : InteractablePin<InputText>(name, label, color, config.width,
                                 CreateInputConfig(uniqueId(), label, std::move(config))) {}

InputText::Config StringInputPin::CreateInputConfig(const std::string &name, const std::string &label,
                                                    StringInputPin::InputConfig &&config) {
  return {.name = name,
          .label = label,
          .value = config.value,
          .inputType = config.inputType,
          .maxInputLength = config.maxInputLength,
          .eventTrigger = config.eventTrigger};
}

}  // namespace pf::ui::ig::bp