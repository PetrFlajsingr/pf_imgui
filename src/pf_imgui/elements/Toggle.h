//
// Created by petr.flajsingr on 2/9/2022.
//

#ifndef PF_IMGUI_TOGGLE_H
#define PF_IMGUI_TOGGLE_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A toggle working the same as Checkbox.
 *
 * It saves it's state and provides it to listeners.
 */
class PF_IMGUI_EXPORT Toggle
    : public ItemElement,
      public ValueObservable<bool>,
      public Labellable,
      public Savable,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::CheckMark,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::FrameBackground,
                               style::ColorOf::FrameBackgroundHovered, style::ColorOf::NavHighlight,
                               style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  struct Config {
    using Parent = Toggle;
    std::string_view name;
    std::string_view label;
    bool enabled = false;
    Persistent persistent = Persistent::No;
  };
  explicit Toggle(Config &&config);
  /**
   * Construct Toggle.
   * @param elementName ID of the checkbox
   * @param label label drawn next to the checkbox
   * @param persistent allow value saving to disk
   * @param value starting value
   */
  Toggle(const std::string &elementName, const std::string &label, bool value = false,
         Persistent persistent = Persistent::No);
  /**
   * Set if the checkbox is selected or not.
   * @param selected new value
   */
  void setSelected(bool selected);
  /**
   * Check if the checkbox is selected.
   * @return true if the checkbox is selected, false otherwise
   */
  [[nodiscard]] bool isSelected() const;
  /**
   * Toggle the checkboxes state. True becomes false, false becomes true.
   */
  void toggle();

 protected:
  void unserialize_impl(const toml::table &src) override;
  [[nodiscard]] toml::table serialize_impl() const override;

  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_TOGGLE_H
