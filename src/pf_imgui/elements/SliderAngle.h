/**
 * @file SliderAngle.h
 * @brief SliderAngle element.
 * @author Petr Flajšingr
 * @date 8.12.20
 */

#ifndef PF_IMGUI_ELEMENTS_SLIDERANGLE_H
#define PF_IMGUI_ELEMENTS_SLIDERANGLE_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Special slider for angles.
 *
 * Angles are in radians.
 */
class PF_IMGUI_EXPORT SliderAngle
    : public ItemElement,
      public Labellable,
      public ValueObservable<float>,
      public Savable,
      public DragSource<float>,
      public DropTarget<float>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::DragDropTarget,
                               style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::DragDropTarget,
                               style::ColorOf::SliderGrab, style::ColorOf::SliderGrabActive,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  /**
   * Construct SliderAngle.
   * @param elementName ID of the slider
   * @param label text rendered next to the slider
   * @param min min rad angle
   * @param max max rad angle
   * @param value starting rad value
   * @param persistent enable state saving to disk
   * @param format printf-like format for rendering current value over the slider
   */
  SliderAngle(const std::string &elementName, const std::string &label, float min, float max, float value = 0.0,
              Persistent persistent = Persistent::No, std::string format = "%.0f deg");

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

 protected:
  void renderImpl() override;
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

 private:
  float minDeg;
  float maxDeg;
  std::string format;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_SLIDERANGLE_H
