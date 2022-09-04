/**
 * @file Selectable.h
 * @brief Selectable element.
 * @author Petr Flajšingr
 * @date 22.05.21
 */

#ifndef PF_IMGUI_ELEMENTS_SELECTABLE_H
#define PF_IMGUI_ELEMENTS_SELECTABLE_H

#include <pf_common/Explicit.h>
#include <pf_imgui/Font.h>
#include <pf_imgui/Label.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/Size.h>
#include <pf_imgui/reactive/Observable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <pf_imgui/style/common.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A labeled area which can be de/selected.
 *
 * It's basically a different kind of checkbox.
 */
class PF_IMGUI_EXPORT Selectable : public ItemElement, public ValueObservable<bool>, public Savable {
 public:
  /**
   * @brief Struct for construction of Selectable.
   */
  struct Config {
    using Parent = Selectable;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered on top of the element */
    bool selected = false;            /*!< Initial state of the Selectable */
    Size size = Size::Auto();         /*!< Size of the element */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct Selectable
   * @param config construction args @see Selectable::Config
   */
  explicit Selectable(Config &&config);
  /**
   * Construct Selectable.
   * @param elementName ID of the element
   * @param labelText text rendered on the element
   * @param initialValue true for selected, false for not selected
   * @param s size of the element
   * @param persistent allow value saving to disk
   */
  Selectable(const std::string &elementName, const std::string &labelText, bool initialValue = false,
             Size s = Size::Auto(), Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::FrameBackground, ColorOf::Border, ColorOf::BorderShadow,
               ColorOf::FrameBackgroundActive, ColorOf::Header, ColorOf::HeaderHovered, ColorOf::HeaderActive,
               ColorOf::NavHighlight>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize, StyleOf::SelectableTextAlign>
      style;
  Font font = Font::Default();
  Label label;

  Observable<Size> size;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SELECTABLE_H
