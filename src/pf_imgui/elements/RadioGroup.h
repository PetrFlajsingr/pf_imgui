//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_RADIOGROUP_H
#define PF_IMGUI_IMGUI_ELEMENTS_RADIOGROUP_H

#include "RadioButton.h"
#include "interface/Labellable.h"
#include "interface/SavableElement.h"
#include <pf_imgui/_export.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT RadioGroup : public Labellable,
                                   public ValueObservableElement<std::string_view>,
                                   public SavableElement {
 public:
  RadioGroup(const std::string &elementName, const std::string &label, std::vector<RadioButton> buttons = {},
             const std::optional<std::size_t> &selectedButtonIndex = std::nullopt,
             Persistent persistent = Persistent::No);

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

#endif//PF_IMGUI_IMGUI_ELEMENTS_RADIOGROUP_H
