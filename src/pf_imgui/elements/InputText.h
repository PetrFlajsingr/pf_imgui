/**
 * @file InputText.h
 * @brief InputText element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_INPUTTEXT_H
#define PF_IMGUI_ELEMENTS_INPUTTEXT_H

#include <functional>
#include <memory>
#include <pf_common/Explicit.h>
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>

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
class PF_IMGUI_EXPORT InputText : public ItemElement,
                                  public ValueContainer<std::string>,
                                  public Savable,
                                  public DragSource<std::string>,
                                  public DropTarget<std::string> {
 public:
  /**
   * @brief Struct for construction of InputText.
   */
  struct Config {
    using Parent = InputText;
    Explicit<std::string_view> name;                     /*!< Unique name of the element */
    Explicit<std::string_view> label;                    /*!< Text rendered next to the input */
    std::string value{};                                 /*!< Initial value */
    TextInputType inputType = TextInputType::SingleLine; /*!< Type of text input */
    std::size_t maxInputLength = 256;                    /*!< Maximum length of input string */
    TextTrigger eventTrigger = TextTrigger::Character;   /*!< Trigger to notify listeners */
    Flags<TextFilter> filters = TextFilter::None;        /*!< Enabled text filters */
    bool persistent = false;                             /*!< Allow state saving to disk */
  };
  /**
   * Construct InputText
   * @param config construction args @see InputText::Config
   */
  explicit InputText(Config &&config);
  /**
   * Construct InputText.
   * @param elementName ID of the input
   * @param label text rendered next to the input
   * @param initialValue starting text in the input
   * @param textInputType singleline or multiline support
   * @param filters character filters for input
   * @param persistent enable state saving to disk
   */
  InputText(std::string_view elementName, std::string_view label, std::string_view initialValue = "",
            TextInputType textInputType = TextInputType::SingleLine, std::size_t inputLengthLimit = 256,
            TextTrigger trigger = TextTrigger::Character, Flags<TextFilter> filters = TextFilter::None,
            Persistent persistent = Persistent::No);

  /**
   * Clear text.
   */
  void clear();

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
  [[nodiscard]] bool isPassword() const;
  /**
   * Set if the input renders all characters as hidden;
   * @param password
   */
  void setPassword(bool passwd);

  [[nodiscard]] TextInputType getInputType() const;
  void setInputType(TextInputType newInputType);

  [[nodiscard]] TextTrigger getInputTrigger() const;
  void setInputTrigger(TextTrigger trigger);

  [[nodiscard]] Flags<TextFilter> getFilters() const;
  void setFilters(Flags<TextFilter> filters);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  void setValue(const std::string &newValue) override;
  [[nodiscard]] const std::string &getValue() const override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const std::string &)> listener) override;

 public:
  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::FrameBackground,
               ColorOf::FrameBackgroundHovered, ColorOf::FrameBackgroundActive, ColorOf::TextSelectedBackground>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Property<Label> label;
  Property<std::string> text;

 protected:
  void renderImpl() override;

  void setTextInner(std::string txt);

 private:
  std::unique_ptr<char[]> buffer;
  std::size_t bufferLength;
  TextInputType inputType;
  ImGuiInputTextFlags flags = {};
  bool readOnly = false;
  bool password = false;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_INPUTTEXT_H
