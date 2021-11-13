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
 * @todo: style?
 * @todo: link icon by user
 * @todo: tooltip callback
 */
class PF_IMGUI_EXPORT MarkdownText : public ItemElement {
 public:
  struct ImageData {
    ImTextureID textureId;
    Size size;
  };
  using ImageLoader = std::function<std::optional<ImageData>(std::string_view)>;
  struct FontDefault;
  /**
   * Construct MarkdownText.
   * @param elementName ID of the element
   * @param imguiInterface for automatic font upload @todo: change this
   * @param markdownSrc markdown source to render
   * @param fontSize size of font
   * @param imageLoader function for image loading & texture creation, arg is path to load
   */
  explicit MarkdownText(const std::string &elementName, ImGuiInterface &imguiInterface,
                        std::u8string markdownSrc = u8"", float fontSize = 12.f,
                        std::optional<ImageLoader> imageLoader = std::nullopt);

  /**
   * Get currently rendered markdown source.
   * @return currently rendered markdown source
   */
  [[nodiscard]] const std::u8string &getMarkdownSrc() const;
  /**
   * Set markdown source to be rendered
   * @param markdownSrc source to render
   */
  void setMarkdownSrc(const std::u8string &markdownSrc);
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

  /**
   * Set callback for user clicking a link.
   * @param onLinkClicked callback - first argument is link itself, second is true if it is an image
   */
  void setOnLinkClicked(const std::function<void(std::string_view, bool)> &onLinkClicked);

  /**
   * Set function for image loading & texture creation.
   * @param imageLoader function for image loading & texture creation, arg is path to load
   */
  void setImageLoader(ImageLoader imageLoader);

  // TODO: static void SetFont(ImFont *font, ImGuiInterface &imGuiInterface);

 protected:
  void renderImpl() override;

 private:
  void configure();

  void loadHeaderFonts();
  static void MarkdownFormatCallback(const ImGui::MarkdownFormatInfo &markdownFormatInfo, bool start);
  static void MarkdownLinkCallback(ImGui::MarkdownLinkCallbackData data);
  static ImGui::MarkdownImageData MarkdownImageCallback(ImGui::MarkdownLinkCallbackData data);

  static inline struct {
    ImFont *fontH1 = nullptr;
    ImFont *fontH2 = nullptr;
    ImFont *fontH3 = nullptr;
  } FontData;

  ImGui::MarkdownConfig markdownConfig;
  ImGuiInterface &imGuiInterface;
  std::u8string markdownSrc;
  float fontSize = 12.f;
  std::function<void(std::string_view, bool)> onLinkClicked = [](auto, auto) {};
  std::optional<ImageLoader> loadImage = std::nullopt;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_MARKDOWNTEXT_H
