//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_CHECKBOX_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_CHECKBOX_H

#include "interface/LabeledElement.h"
#include "interface/SavableElement.h"
#include "interface/ValueObservableElement.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Checkbox : public ValueObservableElement<bool>,
                                 public LabeledElement,
                                 public SavableElement {
 public:
  Checkbox(const std::string &elementName, const std::string &caption,
           Persistent persistent = Persistent::No, bool value = false);

 protected:
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

 protected:
  void renderImpl() override;
};
}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_CHECKBOX_H
