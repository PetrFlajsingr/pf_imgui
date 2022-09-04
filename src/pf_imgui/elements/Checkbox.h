/**
 * @file Checkbox.h
 * @brief Checkbox element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_CHECKBOX_H
#define PF_IMGUI_ELEMENTS_CHECKBOX_H

#include <pf_common/Explicit.h>
#include <pf_imgui/Font.h>
#include <pf_imgui/Label.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <pf_imgui/style/common.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A typical checkbox.
 *
 * A checkbox which saves it's state and provides it to listeners.
 */
class PF_IMGUI_EXPORT Checkbox : public ItemElement, public ValueObservable<bool>, public Savable {
 public:
  /**
   * @brief Struct for construction of Checkbox.
   */
  struct Config {
    using Parent = Checkbox;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the checkbox */
    bool checked = false;             /*!< Initial state of the checkbox */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct Checkbox
   * @param config construction args @see Checkbox::Config
   */
  explicit Checkbox(Config &&config);
  /**
   * Construct Checkbox.
   * @param elementName ID of the checkbox
   * @param labelText label drawn next to the checkbox
   * @param persistent allow value saving to disk
   * @param initialValue starting value
   */
  Checkbox(const std::string &elementName, const std::string &labelText, bool initialValue = false,
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

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::CheckMark, ColorOf::FrameBackgroundActive,
               ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Observable<Label> label;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_CHECKBOX_H
