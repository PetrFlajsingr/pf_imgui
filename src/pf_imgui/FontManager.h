//
// Created by petr on 6/12/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_INTERFACE_FONTMANAGER_H
#define PF_IMGUI_SRC_PF_IMGUI_INTERFACE_FONTMANAGER_H

#include <experimental/memory>
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

struct FontConfig {
  float fontSize;
  int indexInTTF{};
  float extraHorizontalSpacing{};
  std::vector<FontConfig> fontsToMerge;
};

class FontManager;
class FontBuilder;

class SubFontBuilder {
 public:
  explicit SubFontBuilder(FontBuilder &builder, std::filesystem::path ttfPath);
  explicit SubFontBuilder(FontBuilder &builder, std::vector<std::byte> data);
  SubFontBuilder &setFontSize(float size);
  SubFontBuilder &setIndexInTTF(uint32_t index);
  SubFontBuilder &setExtraHorizontalSpacing(float spacing);
  SubFontBuilder &setGlyphRange(const std::array<ImWchar, 3> &range);
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

class FontBuilder {
 public:
  explicit FontBuilder(FontManager &manager, std::string fontName, std::filesystem::path ttfPath);
  explicit FontBuilder(FontManager &manager, std::string fontName, std::vector<std::byte> data);
  SubFontBuilder addSubfont(const std::filesystem::path &ttfPath);
  SubFontBuilder addSubfont(const std::vector<std::byte> &data);
  FontBuilder &addIconSubfont(Flags<IconPack> iconPack, float size);
  FontBuilder &setFontSize(float size);
  FontBuilder &setIndexInTTF(uint32_t index);
  FontBuilder &setExtraHorizontalSpacing(float spacing);

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

// TODO: this'll get messed up after 100 fonts, cause glyphRangeInfos will realloc and pointers will be lost
class FontManager {
 public:
  explicit FontManager(ImGuiInterface &imGuiInterface, std::filesystem::path iconFontDir);
  explicit FontManager(ImGuiInterface &imGuiInterface, const std::filesystem::path &iconFontDir,
                       Flags<IconPack> iconPacks, float iconSize);

  [[nodiscard]] std::optional<ImFont *> fontByName(const std::string &name) const;
  //void removeFontByName(const std::string &name) const;
  [[nodiscard]] FontBuilder fontBuilder(const std::string &name, const std::filesystem::path &path);
  [[nodiscard]] FontBuilder fontBuilder(const std::string &name, std::vector<std::byte> data);

 private:
  friend class FontBuilder;
  ImFont *addFont(FontBuilder &builder);

  std::experimental::observer_ptr<ImGuiInterface> interface;
  std::filesystem::path iconDir;
  std::unordered_map<std::string, ImFont *> fonts;
  std::vector<std::array<ImWchar, 3>> glyphRangeInfos;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_SRC_PF_IMGUI_INTERFACE_FONTMANAGER_H
