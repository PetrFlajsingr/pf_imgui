//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_RADIOGROUP_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_RADIOGROUP_H

#include "RadioButton.h"
#include "interface/LabeledElement.h"
#include "interface/SavableElement.h"

namespace pf::ui::ig {

class RadioGroup : public LabeledElement,
                   public ValueObservableElement<std::string_view>,
                   public SavableElement {
 public:
  RadioGroup(const std::string &elementName, const std::string &caption,
             std::vector<RadioButton> buttons = {},
             const std::optional<std::size_t> &selectedButtonIndex = std::nullopt, Persistent persistent = Persistent::No);

  void addButton(const std::string &elementName, const std::string &caption, bool value = false);

 protected:
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;
  void renderImpl() override;

 private:
  std::vector<RadioButton> buttons;
  std::optional<std::size_t> selectedButtonIndex = std::nullopt;
};
}// namespace pf::ui::ig

#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_RADIOGROUP_H
