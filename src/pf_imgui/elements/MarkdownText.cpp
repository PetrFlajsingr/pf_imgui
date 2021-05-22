//
// Created by petr on 5/22/21.
//

#include "MarkdownText.h"
#include <utility>

namespace pf::ui::ig {

MarkdownText::MarkdownText(const std::string &elementName, std::string markdownSrc, float fontSize)
    : ItemElement(elementName), markdownSrc(std::move(markdownSrc)), fontSize(fontSize) {
  loadHeaderFonts();
  configure();
}

void MarkdownText::renderImpl() { ImGui::Markdown(markdownSrc.c_str(), markdownSrc.length(), markdownConfig); }

void MarkdownText::loadHeaderFonts() {
  if (FontData.fontH1 != nullptr) {
    FontData.fontH1 = ImGui::GetIO().Fonts->AddFontDefault();
    FontData.fontH1->Scale = fontSize * 1.15f;
    FontData.fontH2 = ImGui::GetIO().Fonts->AddFontDefault();
    FontData.fontH2->Scale = fontSize * 1.1f;
    FontData.fontH3 = ImGui::GetIO().Fonts->AddFontDefault();
    FontData.fontH3->Scale = fontSize * 1.05f;
  }
}

void MarkdownText::MarkdownFormatCallback(const ImGui::MarkdownFormatInfo &markdownFormatInfo, bool start) {
  ImGui::defaultMarkdownFormatCallback(markdownFormatInfo, start);
  // TODO: custom formatting by user
}

void MarkdownText::configure() {
  // TODO: markdownConfig.linkCallback =         LinkCallback;
  markdownConfig.tooltipCallback = nullptr;
  // TODO: markdownConfig.imageCallback =        ImageCallback;
  // TODO: markdownConfig.linkIcon =             ICON_FA_LINK;
  markdownConfig.headingFormats[0] = {FontData.fontH1, true};
  markdownConfig.headingFormats[1] = {FontData.fontH2, true};
  markdownConfig.headingFormats[2] = {FontData.fontH3, false};
  markdownConfig.userData = nullptr;
  markdownConfig.formatCallback = MarkdownFormatCallback;
}

const std::string &MarkdownText::getMarkdownSrc() const { return markdownSrc; }

void MarkdownText::setMarkdownSrc(const std::string &markdown) { markdownSrc = markdown; }

float MarkdownText::getFontSize() const { return fontSize; }
void MarkdownText::setFontSize(float size) {
  fontSize = size;
  loadHeaderFonts();
  configure();
}
}// namespace pf::ui::ig