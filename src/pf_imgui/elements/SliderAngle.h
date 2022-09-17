/**
 * @file SliderAngle.h
 * @brief SliderAngle element.
 * @author Petr Flajšingr
 * @date 8.12.20
 */

#ifndef PF_IMGUI_ELEMENTS_SLIDERANGLE_H
#define PF_IMGUI_ELEMENTS_SLIDERANGLE_H

#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Special slider for angles.
 *
 * Angles are in radians.
 */
class PF_IMGUI_EXPORT SliderAngle : public ItemElement,
                                    public ValueContainer<float>,
                                    public Savable,
                                    public DragSource<float>,
                                    public DropTarget<float> {
 public:
  /**
   * @brief Struct for construction of SliderAngle.
   */
  struct Config {
    using Parent = SliderAngle;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the element */
    Explicit<float> min;              /*!< Minimum allowed value */
    Explicit<float> max;              /*!< Maximum allowed value */
    float value{};                    /*!< Initial value */
    std::string format = "%.0f deg";  /*!< Format string for value rendering */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct SliderAngle
   * @param config construction args @see SliderAngle::Config
   */
  explicit SliderAngle(Config &&config);
  /**
   * Construct SliderAngle.
   * @param elementName ID of the slider
   * @param labelText text rendered next to the slider
   * @param min min rad angle
   * @param max max rad angle
   * @param initialValue starting rad value
   * @param persistent enable state saving to disk
   * @param numberFormat printf-like format for rendering current value over the slider
   */
  SliderAngle(std::string_view elementName, std::string_view labelText, float min, float max, float initialValue = 0.0,
              Persistent persistent = Persistent::No, std::string numberFormat = "%.0f deg");

  /**
   * Get min slider value.
   * @return min slider value
   */
  [[nodiscard]] float getMinDeg() const;
  /**
   * Set min slider value.
   * @param minDeg new min slider value
   */
  void setMinDeg(float min);
  /**
   * Get max slider value.
   * @return max slider value
   */
  [[nodiscard]] float getMaxDeg() const;
  /**
   * Set max slider value.
   * @param maxDeg new min slider value
   */
  void setMaxDeg(float max);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::FrameBackground,
               ColorOf::FrameBackgroundHovered, ColorOf::FrameBackgroundActive, ColorOf::DragDropTarget,
               ColorOf::SliderGrab, ColorOf::SliderGrabActive, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Property<Label> label;
  Property<float> angle;

  [[nodiscard]] const float &getValue() const override;
  void setValue(const float &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const float &)> listener) override;

  void renderImpl() override;

 private:
  float minDeg;
  float maxDeg;
  std::string format;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SLIDERANGLE_H
