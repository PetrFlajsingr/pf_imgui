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
class PF_IMGUI_EXPORT Button : public ItemElement, public Labellable, public Resizable, public Clickable {
 public:
  /**
   * Construct Button.
   * @param name ID of the button
   * @param label label drawn on the button
   * @param buttonType type of the button
   * @param size size of the button
   */
  Button(const std::string &name, std::string label, ButtonType buttonType = ButtonType::Normal,
         const ImVec2 &size = {0, 0});

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

 protected:
  void renderImpl() override;

 private:
  ButtonType type;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_BUTTON_H
