//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INPUTTEXT_H
#define PF_IMGUI_IMGUI_ELEMENTS_INPUTTEXT_H

#include "Text.h"
#include "interface/ItemElement.h"
#include "interface/Labellable.h"
#include "interface/SavableElement.h"
#include "interface/ValueObservableElement.h"
#include <functional>
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT InputText : public Text,
                                  public Labellable,
                                  public ValueObservableElement<std::string_view>,
                                  public SavableElement {
 public:
  InputText(const std::string &elementName, std::string label, const std::string &text = "",
            TextInputType textInputType = TextInputType::SingleLine, Persistent persistent = Persistent::No);

  void clear();

 protected:
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

  void renderImpl() override;

 private:
  char buffer[256]{};
  TextInputType inputType;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INPUTTEXT_H
