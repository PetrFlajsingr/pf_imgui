//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_RADIOGROUP_H
#define PF_IMGUI_ELEMENTS_RADIOGROUP_H

#include "RadioButton.h"
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Group of RadioButton.
 *
 * Only one button in a group can be selected at one time.
 *
 * Label of the selected button is observable.
 *
 * @todo: existing button to group addition support
 */
class PF_IMGUI_EXPORT RadioGroup : public Element,
                                   public Labellable,
                                   public ValueObservable<std::string_view>,
                                   public Savable {
 public:
  /**
   * Construct RadioGroup.
   * @param elementName ID of the element
   * @param label text rendered above the group
   * @param buttons buttons in the group
   * @param selectedButtonIndex index of selected button
   * @param persistent enable state saving to disk
   */
  RadioGroup(const std::string &elementName, const std::string &label, std::vector<RadioButton> buttons = {},
             const std::optional<std::size_t> &selectedButtonIndex = std::nullopt,
             Persistent persistent = Persistent::No);

  /**
   * Create a new button and add it to the group.
   * @param elementName ID of the button
   * @param caption text rendered next to the button
   * @param value true for selected, false otherwise
   */
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

#endif//PF_IMGUI_ELEMENTS_RADIOGROUP_H
