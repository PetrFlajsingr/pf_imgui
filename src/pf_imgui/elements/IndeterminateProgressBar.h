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
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Progress bar with infinite animation.
 */
class PF_IMGUI_EXPORT IndeterminateProgressBar
    : public ItemElement,
      public Resizable,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::FrameBackground,
                               style::ColorOf::FrameBackgroundHovered, style::ColorOf::FrameBackgroundActive,
                               style::ColorOf::Border, style::ColorOf::BorderShadow, style::ColorOf::PlotHistogram>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  /**
   * Construct IndeterminateProgressBar
   * @param config construction args
   */
  struct Config {
    using Parent = IndeterminateProgressBar;
    std::string_view name;    /*!< Unique name of the element */
    float speed = 1.f;        /*!< Speed of change */
    std::string overlay = ""; /*!< Text rendered on top of the element */
    Size size = Size::Auto(); /*!< Size of the element */
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
   * @param size size of the progress bar
   */
  explicit IndeterminateProgressBar(const std::string &elementName, std::string overlayStr = "", float speed = 1.f,
                                    const Size &size = Size::Auto());

  /**
   * @param speed new speed of change
   */
  void setSpeed(float speed);
  /**
   * @return current speed of change
   */
  [[nodiscard]] float getSpeed() const;

 protected:
  void renderImpl() override;

 private:
  float changeSpeed;
  std::string overlay;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_INDETERMINATEPROGRESSBAR_H
