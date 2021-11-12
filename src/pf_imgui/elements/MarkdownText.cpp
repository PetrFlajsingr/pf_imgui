//
// Created by petr on 5/22/21.
//

#include "MarkdownText.h"
#include <utility>

namespace pf::ui::ig {

MarkdownText::MarkdownText(const std::string &elementName, ImGuiInterface &imguiInterface, std::u8string markdownSrc,
                           float fontSize,
                           std::optional<std::function<std::optional<ImTextureID>(std::string_view)>> &&imageLoader)
    : ItemElement(elementName), imGuiInterface(imguiInterface), markdownSrc(std::move(markdownSrc)), fontSize(fontSize),
      loadImage(imageLoader) {
  loadHeaderFonts();
  configure();
}

void MarkdownText::renderImpl() {
  ImGui::Markdown(reinterpret_cast<const char *>(markdownSrc.data()), markdownSrc.length(), markdownConfig);
}

void MarkdownText::loadHeaderFonts() {
  if (FontData.fontH1 == nullptr) {
    FontData.fontH1 = ImGui::GetIO().Fonts->AddFontDefault();
    FontData.fontH1->Scale = 1.5f;
    FontData.fontH2 = ImGui::GetIO().Fonts->AddFontDefault();
    FontData.fontH2->Scale = 1.30f;
    FontData.fontH3 = ImGui::GetIO().Fonts->AddFontDefault();
    FontData.fontH3->Scale = 1.15f;
    imGuiInterface.updateFonts();
  }
}

void MarkdownText::MarkdownFormatCallback(const ImGui::MarkdownFormatInfo &markdownFormatInfo, bool start) {
  ImGui::defaultMarkdownFormatCallback(markdownFormatInfo, start);
  // TODO: custom formatting by user
}

void MarkdownText::configure() {
  markdownConfig.linkCallback = MarkdownLinkCallback;
  markdownConfig.tooltipCallback = nullptr;
  markdownConfig.imageCallback = MarkdownImageCallback;
  // TODO: markdownConfig.linkIcon =             ICON_FA_LINK;
  markdownConfig.headingFormats[0] = {FontData.fontH1, true};
  markdownConfig.headingFormats[1] = {FontData.fontH2, true};
  markdownConfig.headingFormats[2] = {FontData.fontH3, false};
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
void MarkdownText::setOnLinkClicked(const std::function<void(std::string_view, bool)> &linkClicked) {
  onLinkClicked = linkClicked;
}
ImGui::MarkdownImageData MarkdownText::MarkdownImageCallback(ImGui::MarkdownLinkCallbackData data) {
  auto result = ImGui::MarkdownImageData{};
  result.useLinkCallback = false;
  auto self = reinterpret_cast<MarkdownText *>(data.userData);
  if (!self->loadImage.has_value()) {
    result.isValid = false;
    return result;
  }
  const auto image = (*self->loadImage)(std::string_view(data.link, data.linkLength));
  if (!image.has_value()) {
    result.isValid = false;
    return result;
  }
  result.isValid = true;
  result.user_texture_id = *image;
  // TODO: size
  result.size = ImVec2(40.0f, 20.0f);

  ImVec2 const contentSize = ImGui::GetContentRegionAvail();
  if (result.size.x > contentSize.x) {
    float const ratio = result.size.y / result.size.x;
    result.size.x = contentSize.x;
    result.size.y = contentSize.x * ratio;
  }

  return result;
}
void MarkdownText::setImageLoader(std::function<std::optional<ImTextureID>(std::string_view)> &&imageLoader) {
  loadImage = imageLoader;
}
}// namespace pf::ui::ig