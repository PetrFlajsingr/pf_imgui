//
// Created by xflajs00 on 11.11.2021.
//

#ifndef PF_IMGUI_SPINNER_H
#define PF_IMGUI_SPINNER_H

#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/styles/enums.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief A spinner notifying the user of an operation in progress.
 */
class PF_IMGUI_EXPORT Spinner : public ItemElement,
                                public ColorCustomizable<style::ColorOf::PlotHistogram>,
                                public StyleCustomizable<style::Style::FramePadding> {
 public:
  /**
   * @brief Struct for construction of Checkbox.
   */
  struct Config {
    using Parent = Spinner;
    std::string_view name; /*!< Unique name of the element */
    float radius;          /*<! Radius of the Spinner */
    int thickness;         /*!< Thickness of rendered spinner line */
  };
  /**
   * Construct Spinner
   * @param config construction args @see Spinner::Config
   */
  explicit Spinner(Config &&config);
  /**
   *
   * @param elementName unique element name
   * @param radius radius of the spin circle
   * @param thickness thickness of the spin line
   */
  Spinner(const std::string &elementName, float radius, int thickness);

 protected:
  void renderImpl() override;

 private:
  float radius;
  int thickness;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SPINNER_H
