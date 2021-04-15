//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_TEXT_H
#define PF_IMGUI_IMGUI_ELEMENTS_TEXT_H

#include <imgui.h>
#include <optional>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/ItemElement.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Simple rendered text. May be colored.
 */
class PF_IMGUI_EXPORT Text : public ItemElement {
 public:
  /**
   * Construct Text.
   * @param elementName ID of the element
   * @param text text to be rendered
   */
  Text(const std::string &elementName, std::string text);
  /**
   * Construct Text.
   * @param elementName ID of the element
   * @param text text to be rendered
   * @param textColor color of the text RGBA
   */
  Text(const std::string &elementName, std::string text, ImVec4 textColor);

  /**
   * Get rendered text.
   * @return text
   */
  [[nodiscard]] const std::string &getText() const;
  /**
   * Set new text for rendering
   * @param text new text to set
   * @param args values to insert into text using fmt::format
   */
  void setText(const std::string &fmt, auto &&...args) {
    setText(fmt::format(fmt, std::forward<decltype(args)>(args)...));
  }

 protected:
  void renderImpl() override;

 private:
  std::string text;
  std::optional<ImVec4> color;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_TEXT_H
