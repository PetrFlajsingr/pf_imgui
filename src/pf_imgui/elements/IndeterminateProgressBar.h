/**
 * @file IndeterminateProgressBar.h
 * @brief IndeterminateProgressBar element taken from https://github.com/gan74/imgui
 * @author Petr Flajšingr
 * @date 21.5.22
 */

#ifndef PF_IMGUI_ELEMENTS_INDETERMINATEPROGRESSBAR_H
#define PF_IMGUI_ELEMENTS_INDETERMINATEPROGRESSBAR_H

#include <algorithm>
#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/reactive/Observable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Progress bar with infinite animation.
 */
class PF_IMGUI_EXPORT IndeterminateProgressBar : public ItemElement {
 public:
  /**
   * Construct IndeterminateProgressBar
   * @param config construction args
   */
  struct Config {
    using Parent = IndeterminateProgressBar;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    float speed = 1.f;               /*!< Speed of change */
    std::string overlay{};           /*!< Text rendered on top of the element */
    Size size = Size::Auto();        /*!< Size of the element */
  };
  /**
   * Construct IndeterminateProgressBar
   * @param config construction args @see IndeterminateProgressBar::Config
   */
  explicit IndeterminateProgressBar(Config &&config);
  /**
   * Construct IndeterminateProgressBar.
   * @param elementName ID of the progress bar
   * @param overlayStr text rendered on top of the element
   * @param speed speed of change
   * @param initialSize size of the progress bar
   */
  explicit IndeterminateProgressBar(const std::string &elementName, std::string overlayStr = "", float speed = 1.f,
                                    const Size &initialSize = Size::Auto());

  /**
   * @param speed new speed of change
   */
  void setSpeed(float speed);
  /**
   * @return current speed of change
   */
  [[nodiscard]] float getSpeed() const;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::Border, ColorOf::BorderShadow, ColorOf::PlotHistogram>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();

  Observable<Size> size;

 protected:
  void renderImpl() override;

 private:
  float changeSpeed;
  std::string overlay;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_INDETERMINATEPROGRESSBAR_H
