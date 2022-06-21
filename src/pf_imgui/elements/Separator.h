/**
 * @file Separator.h
 * @brief Visually split UI.
 * @author Petr Flajšingr
 * @date 15.5.21
 */

#ifndef PF_IMGUI_ELEMENTS_SPLITTER_H
#define PF_IMGUI_ELEMENTS_SPLITTER_H

#include <pf_common/Explicit.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/style/ColorPalette.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Visual separation of UI.
 *
 * Automatically detects if it should be horizontal or vertical.
 */
class PF_IMGUI_EXPORT Separator : public Element {
 public:
  /**
   * @brief Struct for construction of Separator.
   */
  struct Config {
    using Parent = Separator;
    Explicit<std::string_view> name; /*!< Unique name of the element */
  };
  /**
   * Construct Separator
   * @param config construction args @see Checkbox::Separator
   */
  explicit Separator(Config &&config);
  /**
   * Construct Separator.
   * @param name element ID
   */
  explicit Separator(const std::string &name);

  ColorPalette<ColorOf::Separator, ColorOf::SeparatorHovered, ColorOf::SeparatorActive> color;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SPLITTER_H
