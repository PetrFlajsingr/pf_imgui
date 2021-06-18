/**
 * @file Selectable.h
 * @brief Selectable element.
 * @author Petr Flajšingr
 * @date 22.05.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SELECTABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SELECTABLE_H

#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

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
   * Construct Selectable.
   * @param elementName ID of the element
   * @param label text rendered on the element
   * @param value true for selected, false for not selected
   * @param s size of the element
   * @param persistent allow value saving to disk
   */
  Selectable(const std::string &elementName, const std::string &label, bool value = false, const Size &s = Size::Auto(),
             Persistent persistent = Persistent::No);

 protected:
  void renderImpl() override;
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SELECTABLE_H
