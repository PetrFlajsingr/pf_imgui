//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_INPUTTEXT_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_INPUTTEXT_H

#include "Text.h"
#include "interface/LabeledElement.h"
#include "interface/SavableElement.h"
#include "interface/ValueObservableElement.h"
#include <functional>
#include <imgui.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

enum class TextInputType { SingleLine, MultiLine };

class PF_IMGUI_EXPORT InputText : public Text,
                  public LabeledElement,
                  public ValueObservableElement<std::string_view>,
                  public SavableElement {
 public:
  InputText(const std::string &elementName, std::string caption, const std::string &text = "",
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
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_INPUTTEXT_H
