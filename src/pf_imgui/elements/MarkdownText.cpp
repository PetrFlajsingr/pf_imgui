//
// Created by petr on 5/22/21.
//

#include "MarkdownText.h"
#include <utility>

namespace pf::ui::ig {

MarkdownText::MarkdownText(MarkdownText::Config &&config)
    : ItemElement(std::string{config.name.value}), imGuiInterface(config.imguiInterface),
      markdownSrc(std::move(config.markdownText)), fontSize(config.fontSize), loadImage(std::move(config.imageLoader)) {
  loadHeaderFonts();
  configure();
}

MarkdownText::MarkdownText(const std::string &elementName, ImGuiInterface &imguiInterface, std::u8string markdownSrc,
                           float fontSize, std::optional<ImageLoader> imageLoader)
    : ItemElement(elementName), imGuiInterface(imguiInterface), markdownSrc(std::move(markdownSrc)), fontSize(fontSize),
      loadImage(std::move(imageLoader)) {
  loadHeaderFonts();
  configure();
}

void MarkdownText::renderImpl() {
  ImGui::Markdown(reinterpret_cast<const char *>(markdownSrc.data()), markdownSrc.length(), markdownConfig);
}

void MarkdownText::loadHeaderFonts() {
  if (details::DefaultFontData.fontH1 == nullptr) {
    const auto fonts = ImGui::GetIO().Fonts;
    details::DefaultFontData.fontH1 = fonts->AddFontDefault();
    details::DefaultFontData.fontH1->Scale = 1.5f;
    details::DefaultFontData.fontH2 = fonts->AddFontDefault();
    details::DefaultFontData.fontH2->Scale = 1.30f;
    details::DefaultFontData.fontH3 = fonts->AddFontDefault();
    details::DefaultFontData.fontH3->Scale = 1.15f;
    imGuiInterface.updateFonts();
  }
}

void MarkdownText::MarkdownFormatCallback(const ImGui::MarkdownFormatInfo &markdownFormatInfo, bool start) {
  ImGui::defaultMarkdownFormatCallback(markdownFormatInfo, start);
  // TODO: custom formatting by user for HEADING for example
}

void MarkdownText::configure() {
  markdownConfig.linkCallback = MarkdownLinkCallback;
  markdownConfig.tooltipCallback = nullptr;
  markdownConfig.imageCallback = MarkdownImageCallback;
  // TODO: markdownConfig.linkIcon =             ICON_FA_LINK;
  markdownConfig.headingFormats[0] = {details::DefaultFontData.fontH1, true};
  markdownConfig.headingFormats[1] = {details::DefaultFontData.fontH2, true};
  markdownConfig.headingFormats[2] = {details::DefaultFontData.fontH3, false};
  markdownConfig.userData = this;
  markdownConfig.formatCallback = MarkdownFormatCallback;
}

const std::u8string &MarkdownText::getMarkdownSrc() const { return markdownSrc; }

void MarkdownText::setMarkdownSrc(const std::u8string &markdown) { markdownSrc = markdown; }

float MarkdownText::getFontSize() const { return fontSize; }

void MarkdownText::setFontSize(float size) {
  fontSize = size;
  loadHeaderFonts();
  configure();
}

void MarkdownText::MarkdownLinkCallback(ImGui::MarkdownLinkCallbackData data) {
  reinterpret_cast<MarkdownText *>(data.userData)
      ->onLinkClicked(std::string_view(data.link, data.linkLength), data.isImage);
}

ImGui::MarkdownImageData MarkdownText::MarkdownImageCallback(ImGui::MarkdownLinkCallbackData data) {
  auto result = ImGui::MarkdownImageData{};
  result.useLinkCallback = false;
  auto self = reinterpret_cast<MarkdownText *>(data.userData);
  if (!self->loadImage.has_value()) {
    result.isValid = false;
    return result;
  }
  const auto imageData = (*self->loadImage)(std::string_view(data.link, data.linkLength));
  if (!imageData.has_value()) {
    result.isValid = false;
    return result;
  }
  result.isValid = true;
  result.user_texture_id = imageData->textureId;

  result.size = static_cast<ImVec2>(imageData->size);

  ImVec2 const contentSize = ImGui::GetContentRegionAvail();
  if (result.size.x > contentSize.x) {
    float const ratio = result.size.y / result.size.x;
    result.size.x = contentSize.x;
    result.size.y = contentSize.y * ratio;
  }

  return result;
}

void MarkdownText::setImageLoader(ImageLoader imageLoader) { loadImage = imageLoader; }
/* TODO
void MarkdownText::SetFont(ImFont *font, ImGuiInterface &imGuiInterface) {
  details::DefaultFontData.fontH1 = font;
  details::DefaultFontData.fontH1->Scale = 1.5f;
  details::DefaultFontData.fontH2 = font;
  details::DefaultFontData.fontH2->Scale = 1.30f;
  details::DefaultFontData.fontH3 = font;
  details::DefaultFontData.fontH3->Scale = 1.15f;
  imGuiInterface.updateFonts();
}*/

}  // namespace pf::ui::ig
