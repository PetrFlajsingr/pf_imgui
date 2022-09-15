//
// Created by petr.flajsingr on 2/9/2022.
//

#ifndef PF_IMGUI_ELEMENTS_TOGGLE_H
#define PF_IMGUI_ELEMENTS_TOGGLE_H

#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <pf_imgui/style/common.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A toggle working the same as Checkbox.
 *
 * It saves it's state and provides it to listeners.
 */
class PF_IMGUI_EXPORT Toggle : public ItemElement, public ValueContainer<bool>, public Savable {
 public:
  /**
   * @brief Struct for construction of Toggle.
   */
  struct Config {
    using Parent = Toggle;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the element */
    bool enabled = false;             /*!< Initial state */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct Toggle
   * @param config construction args @see Toggle::Config
   */
  explicit Toggle(Config &&config);
  /**
   * Construct Toggle.
   * @param elementName ID of the checkbox
   * @param labelText label drawn next to the checkbox
   * @param initialValue starting value
   * @param persistent allow value saving to disk
   */
  Toggle(std::string_view elementName, std::string_view labelText, bool initialValue = false,
         Persistent persistent = Persistent::No);
  /**
   * Toggle the checkboxes state. True becomes false, false becomes true.
   */
  void toggle();

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  void setValue(const bool &newValue) override;
  [[nodiscard]] const bool &getValue() const override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const bool &)> listener) override;

 public:
  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::CheckMark, ColorOf::FrameBackgroundActive,
               ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Observable<Label> label;
  ObservableProperty<Toggle, bool> selected;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_TOGGLE_H
