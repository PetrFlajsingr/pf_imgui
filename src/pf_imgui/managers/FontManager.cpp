//
// Created by petr on 6/12/21.
//

#include "FontManager.h"
#include <pf_common/Visitor.h>
#include <pf_imgui/ImGuiInterface.h>
#include <range/v3/range/conversion.hpp>
#include <ranges>
#include <string>
#include <utility>
#include <variant>

namespace pf::ui::ig {

FontManager::FontManager(ImGuiInterface &imGuiInterface, std::filesystem::path iconFontDir)
    : imguiInterface(&imGuiInterface), iconDir(std::move(iconFontDir)) {
  glyphRangeInfos.reserve(100);
  auto defaultFont = imguiInterface->getIo().Fonts->AddFontDefault();
  fonts.emplace("default", defaultFont);
}

FontManager::FontManager(ImGuiInterface &imGuiInterface, const std::filesystem::path &iconFontDir,
                         const Flags<IconPack> &iconPacks, float iconSize)
    : FontManager(imGuiInterface, iconFontDir) {
  std::ranges::for_each(iconPacks.getSetFlags(), [&](const auto iconPack) {
    const auto fileNames = fontFileNamesForIconPack(iconPack);
    std::ranges::for_each(fileNames, [&](const auto fileName) {
      const auto fontConfig = fontConfigForIconPack(iconPack);
      const auto fontPath = iconDir / fileName;
      imguiInterface->getIo().Fonts->AddFontFromFileTTF(fontPath.string().c_str(), iconSize, &fontConfig.config,
                                                        fontConfig.iconRange);
    });
  });
}

std::optional<Font> FontManager::fontByName(const std::string &name) const {
  if (auto iter = fonts.find(name); iter != fonts.end()) { return Font{iter->second}; }
  return std::nullopt;
}

FontBuilder FontManager::fontBuilder(const std::string &name, const std::filesystem::path &path) {
  return {*this, name, path};
}

FontBuilder FontManager::fontBuilder(const std::string &name, std::vector<std::byte> data) {
  return {*this, name, std::move(data)};
}

FontBuilder FontManager::fontBuilder(const std::string &name) { return {*this, name}; }

Font FontManager::addFont(FontBuilder &builder) {
  auto fontConfig = ImFontConfig{};
  fontConfig.SizePixels = builder.fontSize;
  fontConfig.GlyphExtraSpacing = ImVec2{builder.extraHorizontalSpacing, 0};
  fontConfig.FontNo = builder.indexInTTF;
  auto font = std::visit<ImFont *>(Visitor{[&, this](const std::filesystem::path &src) {
                                             return imguiInterface->getIo().Fonts->AddFontFromFileTTF(
                                                 src.string().c_str(), builder.fontSize, &fontConfig);
                                           },
                                           [&, this](std::vector<std::byte> &src) {
                                             return imguiInterface->getIo().Fonts->AddFontFromMemoryTTF(
                                                 reinterpret_cast<void *>(src.data()), static_cast<int>(src.size()),
                                                 builder.fontSize, &fontConfig);
                                           },
                                           [&, this](FontBuilder::DefaultFontTag) {
                                             return imguiInterface->getIo().Fonts->AddFontDefault(&fontConfig);
                                           }},
                                   builder.source);
  for (auto &subfontBuilder : builder.subfonts) {
    auto subfontConfig = ImFontConfig{};
    subfontConfig.SizePixels = subfontBuilder.fontSize;
    subfontConfig.GlyphExtraSpacing = ImVec2{subfontBuilder.extraHorizontalSpacing, 0};
    subfontConfig.MergeMode = true;
    subfontConfig.FontNo = subfontBuilder.indexInTTF;
    auto glyphRangePtr = glyphRangeInfos.emplace_back(subfontBuilder.glyphRange).data();
    subfontConfig.GlyphRanges = glyphRangePtr;
    font = std::visit<ImFont *>(Visitor{[&, this](const std::filesystem::path &src) {
                                          return imguiInterface->getIo().Fonts->AddFontFromFileTTF(
                                              src.string().c_str(), subfontBuilder.fontSize, &subfontConfig);
                                        },
                                        [&, this](std::vector<std::byte> &src) {
                                          return imguiInterface->getIo().Fonts->AddFontFromMemoryTTF(
                                              reinterpret_cast<void *>(src.data()), static_cast<int>(src.size()),
                                              subfontBuilder.fontSize, &subfontConfig);
                                        }},
                                subfontBuilder.source);
  }
  std::ranges::for_each(builder.iconPacks, [&](const auto iconPackSize) {
    const auto &[iconPack, iconSize] = iconPackSize;
    const auto fileNames = fontFileNamesForIconPack(iconPack);
    std::ranges::for_each(fileNames, [&](const auto fileName) {
      const auto fontConfig = fontConfigForIconPack(iconPack);
      const auto fontPath = iconDir / fileName;
      imguiInterface->getIo().Fonts->AddFontFromFileTTF(fontPath.string().c_str(), iconSize, &fontConfig.config,
                                                        fontConfig.iconRange);
    });
  });
  fonts.emplace(builder.name, font);
  imguiInterface->shouldUpdateFontAtlas = true;
  return Font{font};
}

SubFontBuilder::SubFontBuilder(FontBuilder &builder, std::filesystem::path ttfPath)
    : parent(builder), source(std::move(ttfPath)) {}

SubFontBuilder::SubFontBuilder(FontBuilder &builder, std::vector<std::byte> data)
    : parent(builder), source(std::move(data)) {}

SubFontBuilder &SubFontBuilder::setFontSize(float size) {
  fontSize = size;
  return *this;
}

SubFontBuilder &SubFontBuilder::setIndexInTTF(uint32_t index) {
  indexInTTF = static_cast<int>(index);
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

FontBuilder::FontBuilder(FontManager &manager, std::string fontName)
    : parent(manager), name(std::move(fontName)), source(FontBuilder::DefaultFontTag{}) {}

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
  indexInTTF = static_cast<int>(index);
  return *this;
}

FontBuilder &FontBuilder::setExtraHorizontalSpacing(float spacing) {
  extraHorizontalSpacing = spacing;
  return *this;
}

Font FontBuilder::build() { return parent.addFont(*this); }

FontBuilder &FontBuilder::addIconSubfont(const Flags<IconPack> &iconPack, float size) {
  iconPacks = iconPack.getSetFlags()
      | std::views::transform([size](const auto pack) { return std::make_pair(pack, size); }) | ranges::to_vector;
  return *this;
}

}  // namespace pf::ui::ig
