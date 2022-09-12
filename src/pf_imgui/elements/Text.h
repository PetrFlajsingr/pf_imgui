/**
 * @file Text.h
 * @brief Text element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_TEXT_H
#define PF_IMGUI_ELEMENTS_TEXT_H

#include <imgui.h>
#include <optional>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/style/ColorPalette.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Simple rendered text. May be colored.
 */
class PF_IMGUI_EXPORT Text : public ItemElement,
                             public ValueContainer<std::string>,
                             public DragSource<std::string>,
                             public DropTarget<std::string> {
 public:
  /**
   * @brief Struct for construction of Checkbox.
   */
  struct Config {
    using Parent = Text;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    std::string text{};              /*!< Text to render */
    bool wrapText = false;           /*!< If true the text will wrap */
  };
  /**
   * Construct Text
   * @param config construction args @see Text::Config
   */
  explicit Text(Config &&config);
  /**
  * Construct Text.
  * @param elementName ID of the element
  * @param textValue text to be rendered
  */
  Text(const std::string &elementName, std::string textValue, bool wrapText = false);

  /**
   * Set new text for rendering
   * @param text new text to set
   * @param args values to insert into text using fmt::format
   */
  template<typename... Args>
  void setText(fmt::format_string<Args...> fmt, Args &&...args) {
    setTextInner(fmt::format(fmt, std::forward<Args>(args)...));
  }

  [[nodiscard]] bool isWrap() const;
  void setWrap(bool textWrap);

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget> color;
  Font font = Font::Default();

  Obser

 protected:
  void renderImpl() override;

  virtual void setTextInner(std::string txt);

 private:
  bool wrap;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_TEXT_H
