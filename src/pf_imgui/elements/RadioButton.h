/**
 * @file RadioButton.h
 * @brief RadioButton element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_RADIOBUTTON_H
#define PF_IMGUI_ELEMENTS_RADIOBUTTON_H

#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/Label.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A button similar in function to CheckBox, but it can belong to RadioGroup. @see RadioGroup
 */
class PF_IMGUI_EXPORT RadioButton : public ItemElement, public ValueObservable<bool> {
 public:
  friend class RadioGroup;
  /**
   * @brief Struct for construction of RadioButton.
   */
  struct Config {
    using Parent = RadioButton;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the radio button */
    bool selected = false;            /*!< Initial state of the button */
  };
  /**
   * Construct RadioButton
   * @param config construction args @see RadioButton::Config
   */
  explicit RadioButton(Config &&config);
  /**
   * Construct RadioButton.
   * @param elementName ID of the button
   * @param label text rendered next to the button
   * @param value starting value of the button
   */
  RadioButton(const std::string &elementName, const std::string &label, bool value = false);

  /**
   * Check if the button is selected.
   * @return true when the button is selected, false otherwise
   */
  [[nodiscard]] bool isSelected() const;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::NavHighlight, ColorOf::CheckMark, ColorOf::Border,
               ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Label label;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_RADIOBUTTON_H
