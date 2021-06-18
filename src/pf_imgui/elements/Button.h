/**
 * @file Button.h
 * @brief Button element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_BUTTON_H
#define PF_IMGUI_ELEMENTS_BUTTON_H

#include <functional>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A typical button element.
 *
 * A button with a text label and an option to change its render type by using enum ButtonType.
 *
 * @see ButtonType
 */
class PF_IMGUI_EXPORT Button
    : public ItemElement,
      public Labellable,
      public Resizable,
      public Clickable,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::Button,
                               style::ColorOf::ButtonHovered, style::ColorOf::ButtonActive>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize,
                               style::Style::ButtonTextAlign> {
 public:
  /**
   * Construct Button.
   * @param name ID of the button
   * @param label label drawn on the button
   * @param buttonType type of the button
   * @param isRepeatable enable/disable button can repeat it's on click event when a user holds it
   * @param size size of the button
   */
  Button(const std::string &name, std::string label, ButtonType buttonType = ButtonType::Normal,
         Repeatable isRepeatable = Repeatable::No, const Size &size = Size::Auto());

  /**
   * Get current type of the button.
   * @return current type of the button
   */
  [[nodiscard]] ButtonType getType() const;
  /**
   * Set new type to the button.
   * @param type new type
   */
  void setType(ButtonType type);

  /**
   * Check whether the button is repeatable or not
   * @return
   */
  [[nodiscard]] bool isRepeatable() const;
  /**
   * Set repeatable. If true then the button can be held by the user to trigger click events.
   * @param repeatable new state
   */
  void setRepeatable(bool repeatable);

 protected:
  void renderImpl() override;

 private:
  ButtonType type;
  bool repeatable;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_BUTTON_H
