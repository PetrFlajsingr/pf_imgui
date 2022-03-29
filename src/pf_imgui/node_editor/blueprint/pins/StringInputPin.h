//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_STRINGINPUTPIN_H
#define IMGUI_EXPERIMENTS_STRINGINPUTPIN_H

#include "InteractablePin.h"
#include <pf_imgui/elements/InputText.h>

namespace pf::ui::ig::bp {

class StringInputPin : public InteractablePin<InputText> {
 public:
  struct InputConfig {
    Width width;
    std::string value{};
    TextInputType inputType = TextInputType::SingleLine;
    std::size_t maxInputLength = 256;
    TextTrigger eventTrigger = TextTrigger::Character;
    Flags<TextFilter> filters = TextFilter::None;
  };

  StringInputPin(const std::string &name, const std::string &label, const ImVec4 &color, InputConfig &&config);

 private:
  static typename InputText::Config CreateInputConfig(const std::string &name, const std::string &label,
                                                      InputConfig &&config);
};

}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_STRINGINPUTPIN_H
