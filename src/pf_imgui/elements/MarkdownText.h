//
// Created by petr on 5/22/21.
//

#ifndef PF_IMGUI_MARKDOWNTEXT_H
#define PF_IMGUI_MARKDOWNTEXT_H

#include <imgui.h>
#include <imgui_markdown.h>
#include <pf_imgui/ImGuiInterface.h>
#include <pf_imgui/interface/ItemElement.h>

namespace pf::ui::ig {

/**
 * @brief Markdown text render area.
 *
 * @todo: custom font
 */
class PF_IMGUI_EXPORT MarkdownText : public ItemElement {
 public:
  /**
   * Construct MarkdownText.
   * @param elementName ID of the element
   * @param interface for automatic font upload @todo: change this
   * @param markdownSrc markdown source to render
   * @param fontSize size of font
   */
  explicit MarkdownText(const std::string &elementName, ImGuiInterface &interface, std::string markdownSrc = "",
                        float fontSize = 12.f);

  /**
   * Get currently rendered markdown source.
   * @return currently rendered markdown source
   */
  [[nodiscard]] const std::string &getMarkdownSrc() const;
  /**
   * Set markdown source to be rendered
   * @param markdownSrc source to render
   */
  void setMarkdownSrc(const std::string &markdownSrc);
  /**
   * Get current font size.
   * @return current font size
   */
  [[nodiscard]] float getFontSize() const;
  /**
   * Set new font size.
   * @param fontSize new font size
   */
  void setFontSize(float fontSize);

 protected:
  void renderImpl() override;

 private:
  void configure();

  void loadHeaderFonts();
  static void MarkdownFormatCallback(const ImGui::MarkdownFormatInfo &markdownFormatInfo, bool start);

  struct {
    ImFont *fontH1 = nullptr;
    ImFont *fontH2 = nullptr;
    ImFont *fontH3 = nullptr;
  } FontData;

  ImGui::MarkdownConfig markdownConfig;
  ImGuiInterface &imGuiInterface;
  std::string markdownSrc;
  float fontSize = 12.f;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_MARKDOWNTEXT_H
