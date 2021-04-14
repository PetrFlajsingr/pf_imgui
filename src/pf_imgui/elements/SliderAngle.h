//
// Created by petr on 12/8/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_SLIDERANGLE_H
#define PF_IMGUI_IMGUI_ELEMENTS_SLIDERANGLE_H

#include <pf_imgui/_export.h>
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
class PF_IMGUI_EXPORT SliderAngle : public ItemElement,
                                    public Labellable,
                                    public ValueObservable<float>,
                                    public Savable {
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

#endif//PF_IMGUI_IMGUI_ELEMENTS_SLIDERANGLE_H
