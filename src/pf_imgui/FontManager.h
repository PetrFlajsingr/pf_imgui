/**
 * @file FontManager.h
 * @brief Class for building and managing imgui fonts.
 * @author Petr Flajšingr
 * @date 12.6.21
 */
#ifndef PF_IMGUI_SRC_PF_IMGUI_INTERFACE_FONTMANAGER_H
#define PF_IMGUI_SRC_PF_IMGUI_INTERFACE_FONTMANAGER_H

#include <memory>
#include <filesystem>
#include <functional>
#include <imgui.h>
#include <optional>
#include <pf_common/enums.h>
#include <pf_imgui/icons.h>
#include <span>
#include <variant>
#include <vector>

namespace pf::ui::ig {

class ImGuiInterface;

class FontManager;
class FontBuilder;

/**
 * @brief Builder for fonts which are merged into another font.
 */
class SubFontBuilder {
 public:
  /**
   * Construct SubFontBuilder.
   * @param builder builder of the main font
   * @param ttfPath ttf file with font data
   */
  explicit SubFontBuilder(FontBuilder &builder, std::filesystem::path ttfPath);
  /**
   * Construct SubFontBuilder.
   * @param builder builder of the main font
   * @param data raw ttf font data
   */
  explicit SubFontBuilder(FontBuilder &builder, std::vector<std::byte> data);
  /**
   * Set size of font.
   * @param size size of font in pixels
   * @return self
   */
  SubFontBuilder &setFontSize(float size);
  /**
   * Set index of font.
   * @param index index of font in ttf data
   * @return self
   */
  SubFontBuilder &setIndexInTTF(uint32_t index);
  /**
   * Set horizontal spacing of font.
   * @param spacing spacing of font in pixels
   * @return self
   */
  SubFontBuilder &setExtraHorizontalSpacing(float spacing);
  /**
   * Set glyph range of subfont.
   * @param range glyph range
   * @return self
   */
  SubFontBuilder &setGlyphRange(const std::array<ImWchar, 3> &range);
  /**
   * Finish building the subfont.
   * @return parent builder
   */
  FontBuilder &endSubfont();

 private:
  friend class FontManager;
  FontBuilder &parent;
  std::variant<std::filesystem::path, std::vector<std::byte>> source;
  float fontSize = 13.f;
  int indexInTTF{};
  float extraHorizontalSpacing{};
  std::array<ImWchar, 3> glyphRange{};
};

/**
 * @brief Builder for fonts.
 */
class FontBuilder {
 public:
  /**
   * Construct FontBuilder.
   * @param manager font manager
   * @param ttfPath path to file containing ttf data
   */
  explicit FontBuilder(FontManager &manager, std::string fontName, std::filesystem::path ttfPath);
  /**
   * Construct FontBuilder.
   * @param manager font manager
   * @param data raw ttf font data
   */
  explicit FontBuilder(FontManager &manager, std::string fontName, std::vector<std::byte> data);
  /**
   * Create a SubFontBuilder.
   * @param ttfPath path to ttf data
   * @return builder for sub font
   */
  SubFontBuilder addSubfont(const std::filesystem::path &ttfPath);
  /**
   * Create a SubFontBuilder.
   * @param data ttf data
   * @return builder for sub font
   */
  SubFontBuilder addSubfont(const std::vector<std::byte> &data);
  /**
   * Add an icon subfont from preset.
   * @param iconPack type of icon pack
   * @param size font size
   * @return self
   */
  FontBuilder &addIconSubfont(Flags<IconPack> iconPack, float size);
  /**
   * Set index of font.
   * @param index index of font in ttf data
   * @return self
   */
  FontBuilder &setFontSize(float size);
  /**
   * Set index of font.
   * @param index index of font in ttf data
   * @return self
   */
  FontBuilder &setIndexInTTF(uint32_t index);
  /**
   * Set horizontal spacing of font.
   * @param spacing spacing of font in pixels
   * @return self
   */
  FontBuilder &setExtraHorizontalSpacing(float spacing);

  /**
   * Build the font.
   * @return built font
   */
  ImFont *build();

 private:
  friend class FontManager;
  friend class SubFontBuilder;
  FontManager &parent;
  std::string name;
  std::variant<std::filesystem::path, std::vector<std::byte>> source;
  float fontSize = 13.f;
  int indexInTTF{};
  float extraHorizontalSpacing{};
  std::vector<SubFontBuilder> subfonts;
  std::vector<std::pair<IconPack, float>> iconPacks;
};

/**
 * @brief Manager for ImGui fonts.
 *
 * @warning: this'll get messed up after 100 fonts, cause glyphRangeInfos will realloc and pointers will be lost
 */
class FontManager {
 public:
  /**
   * Construct font manager with a 'default' font.
   * @param imGuiInterface owner
   * @param iconFontDir directory for default icon fonts
   */
  explicit FontManager(ImGuiInterface &imGuiInterface, std::filesystem::path iconFontDir);
  /**
   * Construct font manager with a 'default' font and icons added to it.
   * @param imGuiInterface owner
   * @param iconFontDir directory for default icon fonts
   * @param iconPacks icons to add to default font
   * @param iconSize size of icons
   */
  explicit FontManager(ImGuiInterface &imGuiInterface, const std::filesystem::path &iconFontDir,
                       Flags<IconPack> iconPacks, float iconSize);

  /**
   * Get font by name.
   * @param name name of the font
   * @return std::nullopt if no font is found, otherwise a pointer to it
   */
  [[nodiscard]] std::optional<ImFont *> fontByName(const std::string &name) const;
  //void removeFontByName(const std::string &name) const;
  /**
   * Build a font from ttf file.
   * @param name name of the font
   * @param path path to ttf file
   * @return builder for font
   */
  [[nodiscard]] FontBuilder fontBuilder(const std::string &name, const std::filesystem::path &path);
  /**
   * Build a font from ttf data.
   * @param name name of the font
   * @param data ttf data
   * @return builder for font
   */
  [[nodiscard]] FontBuilder fontBuilder(const std::string &name, std::vector<std::byte> data);

 private:
  friend class FontBuilder;
  ImFont *addFont(FontBuilder &builder);

  ImGuiInterface *interface;
  std::filesystem::path iconDir;
  std::unordered_map<std::string, ImFont *> fonts;
  std::vector<std::array<ImWchar, 3>> glyphRangeInfos;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_SRC_PF_IMGUI_INTERFACE_FONTMANAGER_H
