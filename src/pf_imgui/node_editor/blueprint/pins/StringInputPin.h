//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_STRINGINPUTPIN_H
#define PF_IMGUI_BLUEPRINT_STRINGINPUTPIN_H

#include "InteractablePin.h"
#include <pf_imgui/elements/InputText.h>
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {

class StringInputPin : public InteractablePin<InputText> {
 public:
  PF_IMGUI_BLUEPRINT_PIN_ID(StringInputPin)
  struct InputConfig {
    Width width;
    std::string value{};
    TextInputType inputType = TextInputType::SingleLine;
    std::size_t maxInputLength = 256;
    TextTrigger eventTrigger = TextTrigger::Character;
    Flags<TextFilter> filters = TextFilter::None;
  };

  StringInputPin(const std::string &elementName, const std::string &labelText, Color pinColor, InputConfig &&config);

  [[nodiscard]] static std::unique_ptr<StringInputPin> ConstructFromToml(ig::Node *parent, const toml::table &src);

  toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 private:
  static typename InputText::Config CreateInputConfig(const std::string &name, const std::string &label,
                                                      InputConfig &&config);
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_STRINGINPUTPIN_H
