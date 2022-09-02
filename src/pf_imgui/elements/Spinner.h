//
// Created by xflajs00 on 11.11.2021.
//

#ifndef PF_IMGUI_ELEMENTS_SPINNER_H
#define PF_IMGUI_ELEMENTS_SPINNER_H

#include <pf_common/Explicit.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <pf_imgui/style/common.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief A spinner notifying the user of an operation in progress.
 */
class PF_IMGUI_EXPORT Spinner : public ItemElement {
 public:
  /**
   * @brief Struct for construction of Checkbox.
   */
  struct Config {
    using Parent = Spinner;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Explicit<float> radius;          /*<! Radius of the Spinner */
    Explicit<int> thickness;         /*!< Thickness of rendered spinner line */
  };
  /**
   * Construct Spinner
   * @param config construction args @see Spinner::Config
   */
  explicit Spinner(Config &&config);
  /**
   *
   * @param elementName unique element name
   * @param spinnerRadius radius of the spin circle
   * @param spinnerThickness thickness of the spin line
   */
  Spinner(const std::string &elementName, float spinnerRadius, int spinnerThickness);

  ColorPalette<ColorOf::PlotHistogram> color;
  StyleOptions<StyleOf::FramePadding> style;

 protected:
  void renderImpl() override;

 private:
  float radius;
  int thickness;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SPINNER_H
