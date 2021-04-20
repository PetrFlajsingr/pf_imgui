/**
 * @file InputText.h
 * @brief InputText element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_INPUTTEXT_H
#define PF_IMGUI_ELEMENTS_INPUTTEXT_H

#include "Text.h"
#include <functional>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Input for text with support for multiline strings.
 *
 * Typical text edit.
 *
 * @todo: filtering
 * @todo: password input
 */
class PF_IMGUI_EXPORT InputText : public Text,
                                  public Labellable,
                                  public ValueObservable<std::string_view>,
                                  public Savable {
 public:
  /**
   * Construct InputText.
   * @param elementName ID of the input
   * @param label text rendered next to the input
   * @param text starting text in the input
   * @param textInputType singleline or multiline support
   * @param persistent enable state saving to disk
   */
  InputText(const std::string &elementName, std::string label, const std::string &text = "",
            TextInputType textInputType = TextInputType::SingleLine, Persistent persistent = Persistent::No);

  /**
   * Clear text.
   */
  void clear();

  void setValue(const std::string_view &newValue) override;

 protected:
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

  void renderImpl() override;

 private:
  char buffer[256]{};
  TextInputType inputType;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_INPUTTEXT_H
