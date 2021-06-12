//
// Created by petr on 6/12/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ICONS_H
#define PF_IMGUI_SRC_PF_IMGUI_ICONS_H

#include <IconsFontAwesome5.h>
#include <IconsForkAwesome.h>
#include <filesystem>

namespace pf::ui::ig {
enum class IconPack { FontAwesome5Regular = 1 << 0,FontAwesome5Solid = 1 << 1, ForkAwesome = 1 << 2 };

inline std::vector<std::filesystem::path> fontFileNamesForIconPack(IconPack iconPack) {
  switch (iconPack) {
    case IconPack::FontAwesome5Regular: return {FONT_ICON_FILE_NAME_FAR};
    case IconPack::FontAwesome5Solid: return {FONT_ICON_FILE_NAME_FAS};
    case IconPack::ForkAwesome: return {FONT_ICON_FILE_NAME_FK};
  }
  return {};
}

namespace details {
static ImWchar iconRangeFA[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
static ImWchar iconRangeFK[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
}// namespace details

struct IconFontConfig {
  ImFontConfig config;
  const ImWchar *iconRange;
};

inline IconFontConfig fontConfigForIconPack(IconPack iconPack) {
  switch (iconPack) {
    case IconPack::FontAwesome5Regular: {
      auto icons_config = ImFontConfig{};
      icons_config.MergeMode = true;
      icons_config.PixelSnapH = true;
      return {icons_config, details::iconRangeFA};
    }
    case IconPack::FontAwesome5Solid: {
      auto icons_config = ImFontConfig{};
      icons_config.MergeMode = true;
      icons_config.PixelSnapH = true;
      return {icons_config, details::iconRangeFA};
    }
    case IconPack::ForkAwesome: {
      auto icons_config = ImFontConfig{};
      icons_config.MergeMode = true;
      icons_config.PixelSnapH = true;
      return {icons_config, details::iconRangeFK};
    }
  }
  return {};
}
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ICONS_H
