//
// Created by petr on 6/12/21.
//

#include "FontManager.h"
#include <pf_common/Visitor.h>
#include <pf_imgui/ImGuiInterface.h>
#include <utility>
#include <variant>

namespace pf::ui::ig {

FontManager::FontManager(ImGuiInterface &imGuiInterface, std::filesystem::path iconFontDir)
    : interface(std::experimental::make_observer(&imGuiInterface)), iconDir(std::move(iconFontDir)) {
  glyphRangeInfos.reserve(100);
  auto defaultFont = interface->getIo().Fonts->AddFontDefault();
  fonts.emplace("default", defaultFont);
}

FontManager::FontManager(ImGuiInterface &imGuiInterface, const std::filesystem::path &iconFontDir,
                         Flags<IconPack> iconPacks, float iconSize)
    : FontManager(imGuiInterface, iconFontDir) {
  std::ranges::for_each(iconPacks.getSetFlags(), [&](const auto iconPack) {
    const auto fileNames = fontFileNamesForIconPack(iconPack);
    std::ranges::for_each(fileNames, [&](const auto fileName) {
      const auto fontConfig = fontConfigForIconPack(iconPack);
      const auto fontPath = iconDir / fileName;
      interface->getIo().Fonts->AddFontFromFileTTF(fontPath.string().c_str(), iconSize, &fontConfig.config,
                                                   fontConfig.iconRange);
    });
  });
}

std::optional<ImFont *> FontManager::fontByName(const std::string &name) const {
  if (auto iter = fonts.find(name); iter != fonts.end()) { return iter->second; }
  return std::nullopt;
}

FontBuilder FontManager::fontBuilder(const std::string &name, const std::filesystem::path &path) {
  return FontBuilder(*this, name, path);
}

FontBuilder FontManager::fontBuilder(const std::string &name, std::vector<std::byte> data) {
  return FontBuilder(*this, name, std::move(data));
}

ImFont *FontManager::addFont(FontBuilder &builder) {
  auto fontConfig = ImFontConfig{};
  fontConfig.SizePixels = builder.fontSize;
  fontConfig.GlyphExtraSpacing = ImVec2{builder.extraHorizontalSpacing, 0};
  auto font = std::visit<ImFont *>(
      Visitor{[&, this](const std::filesystem::path &src) {
                return interface->getIo().Fonts->AddFontFromFileTTF(src.string().c_str(), builder.fontSize,
                                                                    &fontConfig);
              },
              [&, this](std::vector<std::byte> &src) {
                return interface->getIo().Fonts->AddFontFromMemoryTTF(reinterpret_cast<void *>(src.data()), src.size(),
                                                                      builder.fontSize, &fontConfig);
              }},
      builder.source);
  for (auto &subfontBuilder : builder.subfonts) {
    auto subfontConfig = ImFontConfig{};
    subfontConfig.SizePixels = subfontBuilder.fontSize;
    subfontConfig.GlyphExtraSpacing = ImVec2{subfontBuilder.extraHorizontalSpacing, 0};
    auto glyphRangePtr = glyphRangeInfos.emplace_back(subfontBuilder.glyphRange).data();
    subfontConfig.GlyphRanges = glyphRangePtr;
    font = std::visit<ImFont *>(Visitor{[&, this](const std::filesystem::path &src) {
                                          return interface->getIo().Fonts->AddFontFromFileTTF(
                                              src.string().c_str(), subfontBuilder.fontSize, &subfontConfig);
                                        },
                                        [&, this](std::vector<std::byte> &src) {
                                          return interface->getIo().Fonts->AddFontFromMemoryTTF(
                                              reinterpret_cast<void *>(src.data()), src.size(), subfontBuilder.fontSize,
                                              &subfontConfig);
                                        }},
                                subfontBuilder.source);
  }
  std::ranges::for_each(builder.iconPacks, [&](const auto iconPackSize) {
    const auto &[iconPack, iconSize] = iconPackSize;
    const auto fileNames = fontFileNamesForIconPack(iconPack);
    std::ranges::for_each(fileNames, [&](const auto fileName) {
      const auto fontConfig = fontConfigForIconPack(iconPack);
      const auto fontPath = iconDir / fileName;
      interface->getIo().Fonts->AddFontFromFileTTF(fontPath.string().c_str(), iconSize, &fontConfig.config,
                                                   fontConfig.iconRange);
    });
  });
  fonts.emplace(builder.name, font);
  interface->shouldUpdateFontAtlas = true;
  return font;
}

SubFontBuilder::SubFontBuilder(FontBuilder &builder, std::filesystem::path ttfPath)
    : parent(builder), source(ttfPath) {}

SubFontBuilder::SubFontBuilder(FontBuilder &builder, std::vector<std::byte> data) : parent(builder), source(data) {}

SubFontBuilder &SubFontBuilder::setFontSize(float size) {
  fontSize = size;
  return *this;
}

SubFontBuilder &SubFontBuilder::setIndexInTTF(uint32_t index) {
  indexInTTF = index;
  return *this;
}

SubFontBuilder &SubFontBuilder::setExtraHorizontalSpacing(float spacing) {
  extraHorizontalSpacing = spacing;
  return *this;
}

SubFontBuilder &SubFontBuilder::setGlyphRange(const std::array<ImWchar, 3> &range) {
  glyphRange = range;
  return *this;
}

FontBuilder &SubFontBuilder::endSubfont() {
  parent.subfonts.emplace_back(*this);
  return parent;
}

FontBuilder::FontBuilder(FontManager &manager, std::string fontName, std::filesystem::path ttfPath)
    : parent(manager), name(std::move(fontName)), source(ttfPath) {}

FontBuilder::FontBuilder(FontManager &manager, std::string fontName, std::vector<std::byte> data)
    : parent(manager), name(std::move(fontName)), source(std::move(data)) {}

SubFontBuilder FontBuilder::addSubfont(const std::filesystem::path &ttfPath) { return SubFontBuilder{*this, ttfPath}; }

SubFontBuilder FontBuilder::addSubfont(const std::vector<std::byte> &data) { return SubFontBuilder{*this, data}; }

FontBuilder &FontBuilder::setFontSize(float size) {
  fontSize = size;
  return *this;
}

FontBuilder &FontBuilder::setIndexInTTF(uint32_t index) {
  indexInTTF = index;
  return *this;
}

FontBuilder &FontBuilder::setExtraHorizontalSpacing(float spacing) {
  extraHorizontalSpacing = spacing;
  return *this;
}

ImFont *FontBuilder::build() { return parent.addFont(*this); }

FontBuilder &FontBuilder::addIconSubfont(Flags<IconPack> iconPack, float size) {
  iconPacks = iconPack.getSetFlags()
      | std::views::transform([size](const auto pack) { return std::make_pair(pack, size); }) | ranges::to_vector;
  return *this;
}

}// namespace pf::ui::ig