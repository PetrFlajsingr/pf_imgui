/**
 * @file Checkbox.h
 * @brief Checkbox element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_CHECKBOX_H
#define PF_IMGUI_ELEMENTS_CHECKBOX_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A typical checkbox or a toggle.
 *
 * A checkbox which saves it's state and provides it to listeners.
 */
class PF_IMGUI_EXPORT Checkbox
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
  /**
   * Type of the checkbox.
   */
  enum class Type {
    Checkbox, /**< Typical checkbox */
    Toggle    /**< Toggle similiar to those on mobile devices */
  };
  /**
 * Construct Checkbox. Default value is that of Type::Checkbox.
 * @param elementName ID of the checkbox
 * @param label label drawn next to the checkbox
 * @param persistent allow value saving to disk
 * @param value starting value
 */
  Checkbox(const std::string &elementName, const std::string &label, bool value = false,
           Persistent persistent = Persistent::No);
  /**
   * Construct Checkbox.
   * @param elementName ID of the checkbox
   * @param label label drawn next to the checkbox
   * @param checkboxType type of the checkbox
   * @param persistent allow state saving to disk
   * @param value starting value
   */
  Checkbox(const std::string &elementName, const std::string &label, Type checkboxType, bool value = false,
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
  /**
   * Type of checkbox.
   */
  Type type;
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

  void renderImpl() override;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_CHECKBOX_H
