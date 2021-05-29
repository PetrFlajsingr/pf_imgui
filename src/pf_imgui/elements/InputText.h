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
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

enum class TextFilter : uint32_t {
  None,
  Math = 1 << 0,
  Hexadecimal = 1 << 1,
  NoWhiteChar = 1 << 3,
  Scientific = 1 << 17
};

enum class TextTrigger { Character, Enter };

/**
 * @brief Input for text with support for multiline strings.
 *
 * Typical text edit.
 * @todo: hint
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
   * @param filters character filters for input
   * @param persistent enable state saving to disk
   */
  InputText(const std::string &elementName, std::string label, const std::string &text = "",
            TextInputType textInputType = TextInputType::SingleLine, TextTrigger trigger = TextTrigger::Character,
            Flags<TextFilter> filters = TextFilter::None, Persistent persistent = Persistent::No);

  /**
   * Clear text.
   */
  void clear();

  void setValue(const std::string_view &newValue) override;

  /**
   * Check if the input is read only.
   * @return true if read only
   */
  [[nodiscard]] bool isReadOnly() const;
  /**
   * Set if the input is read only.
   * @param readOnly
   */
  void setReadOnly(bool readOnly);

  /**
   * Check if the input renders all characters as hidden.
   * @return
   */
  bool isPassword() const;
  /**
   * Set if the input renders all characters as hidden;
   * @param password
   */
  void setPassword(bool passwd);

 protected:
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

  void renderImpl() override;

  void setTextInner(std::string txt) override;

 private:
  char buffer[256]{};
  TextInputType inputType;
  ImGuiInputTextFlags flags = {};
  bool readOnly = false;
  bool password = false;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_INPUTTEXT_H
