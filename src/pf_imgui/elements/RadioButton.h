/**
 * @file RadioButton.h
 * @brief RadioButton element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_RADIOBUTTON_H
#define PF_IMGUI_ELEMENTS_RADIOBUTTON_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A button similar in function to CheckBox, but it can belong to RadioGroup.
 */
class PF_IMGUI_EXPORT RadioButton
    : public ItemElement,
      public Labellable,
      public ValueObservable<bool>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::FrameBackground,
                               style::ColorOf::FrameBackgroundHovered, style::ColorOf::FrameBackgroundActive,
                               style::ColorOf::NavHighlight, style::ColorOf::CheckMark, style::ColorOf::Border,
                               style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  friend class RadioGroup;
  /**
   * Construct RadioButton.
   * @param elementName ID of the button
   * @param label text rendered next to the button
   * @param value starting value of the button
   */
  RadioButton(const std::string &elementName, const std::string &label, bool value = false);

  /**
   * Check if the button is selected.
   * @return true when the button is selected, false otherwise
   */
  [[nodiscard]] bool isSelected() const;

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_RADIOBUTTON_H
