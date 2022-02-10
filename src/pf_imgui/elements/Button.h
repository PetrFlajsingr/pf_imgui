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
/** Button types in this file: */
class Button;
class SmallButton;
class ArrowButton;
class InvisibleButton;

/**
 * @brief A typical button element.
 *
 * A button with a text label and an option to change its render type by using enum ButtonType.
 *
 * @see ButtonType
 */
class PF_IMGUI_EXPORT ButtonBase : public ItemElement, public Clickable {
 public:
  /**
   * Construct ButtonBase.
   * @param name ID of the button
   * @param isRepeatable enable/disable button can repeat its on click event when a user holds it
   */
  explicit ButtonBase(const std::string &name, Repeatable isRepeatable = Repeatable::No);

  /**
   * Check whether the button is repeatable or not
   * @return
   */
  [[nodiscard]] bool isRepeatable() const;
  /**
   * Set repeatable. If true then the button can be held by the user to trigger click events.
   * @param repeatable new state
   */
  void setRepeatable(bool newRepeatable);

 protected:
  [[nodiscard]] RAII setButtonRepeat();

 private:
  bool repeatable;
};

/**
 * @brief A button which is not rendered, can be used as a part of a new component for click&hover etc. detection.
 */
class PF_IMGUI_EXPORT InvisibleButton : public ButtonBase, public Resizable {
 public:
  struct Config {
    using Parent = InvisibleButton;
    std::string_view name;
    Size size = Size::Auto();
    MouseButton clickButton = MouseButton::Left;
    Repeatable repeatable = Repeatable::No;
  };
  explicit InvisibleButton(Config &&config);
  /**
   * Create InvisibleButton.
   * @param elementName unique name
   * @param s size on screen
   * @param clickButton mouse button to register as a click
   * @param isRepeatable enable/disable button can repeat its on click event when a user holds it
   */
  [[deprecated("Use Config constructor")]] explicit InvisibleButton(const std::string &elementName,
                                                                    const Size &s = Size::Auto(),
                                                                    MouseButton clickButton = MouseButton::Left,
                                                                    Repeatable isRepeatable = Repeatable::No);

 protected:
  void renderImpl() override;

 private:
  MouseButton clickBtn;
};

/**
 * @brief A typical button element.
 *
 * A button with a text label.
 */
class PF_IMGUI_EXPORT Button
    : public ButtonBase,
      public Labellable,
      public Resizable,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::Button,
                               style::ColorOf::ButtonHovered, style::ColorOf::ButtonActive,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize,
                               style::Style::ButtonTextAlign> {
 public:
  struct Config {
    using Parent = Button;
    std::string_view name;
    std::string_view label;
    Size size = Size::Auto();
    Repeatable repeatable = Repeatable::No;
  };
  explicit Button(Config &&config);
  /**
   * Construct Button
   * @param name unique name of the element
   * @param label text rendered on the button
   * @param s size of the button
   * @param isRepeatable if No, then only click notifies, otherwise mouse down repeatedly calls listeners
   */
  Button(const std::string &name, const std::string &label, const Size &s = Size::Auto(),
         Repeatable isRepeatable = Repeatable::No);

 protected:
  void renderImpl() override;
};

/**
 * @brief A small button.
 *
 * A button with a text label.
 */
class PF_IMGUI_EXPORT SmallButton
    : public ButtonBase,
      public Labellable,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::Button,
                               style::ColorOf::ButtonHovered, style::ColorOf::ButtonActive,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize,
                               style::Style::ButtonTextAlign> {
 public:
  struct Config {
    using Parent = SmallButton;
    std::string_view name;
    std::string_view label;
    Repeatable repeatable = Repeatable::No;
  };
  explicit SmallButton(Config &&config);
  /**
   * Construct SmallButton
   * @param name unique name of the element
   * @param label text rendered on the button
   * @param isRepeatable if No, then only click notifies, otherwise mouse down repeatedly calls listeners
   */
  [[deprecated("Use Config constructor")]] SmallButton(const std::string &name, const std::string &label,
                                                       Repeatable isRepeatable = Repeatable::No);

 protected:
  void renderImpl() override;
};
/**
 * @brief A small button with an arrow.
 *
 * A button with an arrow rendered on top of it.
 */
class PF_IMGUI_EXPORT ArrowButton
    : public ButtonBase,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::Button,
                               style::ColorOf::ButtonHovered, style::ColorOf::ButtonActive,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize,
                               style::Style::ButtonTextAlign> {
 public:
  enum class Dir { Up = ImGuiDir_Up, Left = ImGuiDir_Left, Right = ImGuiDir_Right, Down = ImGuiDir_Down };
  struct Config {
    using Parent = ArrowButton;
    std::string_view name;
    Dir direction;
    Repeatable repeatable = Repeatable::No;
  };
  explicit ArrowButton(Config &&config);
  /**
   * Construct ArrowButton
   * @param name unique name of the element
   * @param direction direction of the arrow rendered on the button
   * @param isRepeatable if No, then only click notifies, otherwise mouse down repeatedly calls listeners
   */
  [[deprecated("Use Config constructor")]] ArrowButton(const std::string &name, Dir direction,
                                                       Repeatable isRepeatable = Repeatable::No);

 protected:
  void renderImpl() override;

 private:
  Dir dir;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_BUTTON_H
