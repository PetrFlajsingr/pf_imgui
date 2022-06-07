/**
 * @file Selectable.h
 * @brief Selectable element.
 * @author Petr Flajšingr
 * @date 22.05.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SELECTABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SELECTABLE_H

#include <pf_common/Explicit.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A labeled area which can be de/selected.
 *
 * It's basically a different kind of checkbox.
 */
class PF_IMGUI_EXPORT Selectable
    : public ItemElement,
      public Labellable,
      public ValueObservable<bool>,
      public Resizable,
      public Savable,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::FrameBackground, style::ColorOf::Border,
                               style::ColorOf::BorderShadow, style::ColorOf::FrameBackgroundActive,
                               style::ColorOf::Header, style::ColorOf::HeaderHovered, style::ColorOf::HeaderActive,
                               style::ColorOf::NavHighlight>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize,
                               style::Style::SelectableTextAlign> {
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
   * @param label text rendered on the element
   * @param value true for selected, false for not selected
   * @param s size of the element
   * @param persistent allow value saving to disk
   */
  Selectable(const std::string &elementName, const std::string &label, bool value = false, Size s = Size::Auto(),
             Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SELECTABLE_H
