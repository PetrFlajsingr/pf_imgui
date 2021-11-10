//
// Created by petr on 6/12/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ICONS_H
#define PF_IMGUI_SRC_PF_IMGUI_ICONS_H

#include <IconsFontAwesome5.h>
#include <IconsFontaudio.h>
#include <IconsForkAwesome.h>
#include <IconsKenney.h>
#include <filesystem>

namespace pf::ui::ig {
enum class IconPack {
  FontAwesome5Regular = 1 << 0,
  FontAwesome5Solid = 1 << 1,
  ForkAwesome = 1 << 2,
  Kenney = 1 << 3,
  Fontaudio = 1 << 4
};

inline std::vector<std::filesystem::path> fontFileNamesForIconPack(IconPack iconPack) {
  switch (iconPack) {
    case IconPack::FontAwesome5Regular: return {FONT_ICON_FILE_NAME_FAR};
    case IconPack::FontAwesome5Solid: return {FONT_ICON_FILE_NAME_FAS};
    case IconPack::ForkAwesome: return {FONT_ICON_FILE_NAME_FK};
    case IconPack::Kenney: return {FONT_ICON_FILE_NAME_KI};
    case IconPack::Fontaudio: return {FONT_ICON_FILE_NAME_FAD};
  }
  return {};
}

namespace details {
static ImWchar iconRangeFA[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
static ImWchar iconRangeFK[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
static ImWchar iconRangeKI[] = {ICON_MIN_KI, ICON_MAX_KI, 0};
static ImWchar iconRangeFAD[] = {ICON_MIN_FAD, ICON_MAX_FAD, 0};
}// namespace details

struct IconFontConfig {
  ImFontConfig config;
  const ImWchar *iconRange{};
};

inline IconFontConfig fontConfigForIconPack(IconPack iconPack) {
  auto icons_config = ImFontConfig{};
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  switch (iconPack) {
    case IconPack::FontAwesome5Regular: [[fallthrough]];
    case IconPack::FontAwesome5Solid: {
      return {icons_config, details::iconRangeFA};
    }
    case IconPack::ForkAwesome: {
      return {icons_config, details::iconRangeFK};
    }
    case IconPack::Kenney: {
      return {icons_config, details::iconRangeKI};
    }
    case IconPack::Fontaudio: {
      return {icons_config, details::iconRangeFAD};
    }
  }
  return {};
}
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ICONS_H
