//
// Created by petr on 5/22/21.
//

#ifndef PF_IMGUI_ELEMENTS_MARKDOWNTEXT_H
#define PF_IMGUI_ELEMENTS_MARKDOWNTEXT_H

#include <imgui.h>
#include <imgui_markdown.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/ImGuiInterface.h>
#include <pf_imgui/common/Texture.h>
#include <pf_imgui/interface/ItemElement.h>
#include <string>
#include <utility>

namespace pf::ui::ig {
namespace details {
struct PF_IMGUI_EXPORT FontData {
  ImFont *fontH1 = nullptr;
  ImFont *fontH2 = nullptr;
  ImFont *fontH3 = nullptr;
};
static inline FontData DefaultFontData;
}  // namespace details

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
    std::shared_ptr<Texture> texture;
    Size size;
  };
  using ImageLoader = std::function<std::optional<ImageData>(std::string_view)>;
  struct FontDefault;
  /**
   * @brief Struct for construction of MarkdownText.
   */
  struct Config {
    using Parent = MarkdownText;
    Explicit<std::string_view> name;                       /*!< Unique name of the element */
    ImGuiInterface &imguiInterface;                        /*!< For automatic font upload */
    std::u8string markdownText{};                          /*!< Initial value */
    float fontSize = 12.f;                                 /*!< Size of standard text */
    std::optional<ImageLoader> imageLoader = std::nullopt; /*!< Function for loading image data */
  };
  /**
   * Construct MarkdownText
   * @param config construction args @see MarkdownText::Config
   */
  explicit MarkdownText(Config &&config);
  /**
   * Construct MarkdownText.
   * @param elementName ID of the element
   * @param imguiInterface for automatic font upload @todo: change this
   * @param markdown markdown source to render
   * @param defaultFontSize size of font
   * @param imageLoader function for image loading & texture creation, arg is path to load
   */
  MarkdownText(std::string_view elementName, ImGuiInterface &imguiInterface, std::u8string markdown = u8"",
               float defaultFontSize = 12.f, std::optional<ImageLoader> imageLoader = std::nullopt);

  /**
   * Get currently rendered markdown source.
   * @return currently rendered markdown source
   */
  [[nodiscard]] std::u8string_view getMarkdownSrc() const;
  /**
   * Set markdown source to be rendered
   * @param markdownSrc source to render
   */
  void setMarkdownSrc(std::u8string markdownSrc);
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
   * @param callback callback - first argument is link itself, second is true if it is an image
   */
  void setOnLinkClicked(std::invocable<std::string_view, bool> auto &&callback) {
    onLinkClicked = std::forward<decltype(callback)>(callback);
  }

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

  ImGui::MarkdownConfig markdownConfig;
  ImGuiInterface &imGuiInterface;
  std::u8string markdownSrc{};
  float fontSize = 12.f;
  std::function<void(std::string_view, bool)> onLinkClicked = [](auto, auto) {};
  std::optional<ImageLoader> loadImage = std::nullopt;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_MARKDOWNTEXT_H
