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
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/ItemElement.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
* @brief Simple rendered text. May be colored.
*
* @todo: text wrapping
*/
class PF_IMGUI_EXPORT Text
    : public ItemElement,
      public DragSource<std::string>,
      public DropTarget<std::string>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::DragDropTarget> {
 public:
  /**
   * @brief Struct for construction of Checkbox.
   */
  struct Config {
    using Parent = Text;
    std::string_view name; /*!< Unique name of the element */
    std::string text{};    /*!< Text to render */
    bool wrapText = false;     /*!< If true the text will wrap */
  };
  /**
   * Construct Text
   * @param config construction args @see Text::Config
   */
  explicit Text(Config &&config);
  /**
  * Construct Text.
  * @param elementName ID of the element
  * @param text text to be rendered
  */
  Text(const std::string &elementName, std::string text, bool wrapText = false);

  /**
  * Get rendered text.
  * @return text
  */
  [[nodiscard]] std::string_view getText() const;

  void setText(std::string newText);
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

 protected:
  void renderImpl() override;

  virtual void setTextInner(std::string txt);

 private:
  std::string text;
  bool wrap;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_TEXT_H
