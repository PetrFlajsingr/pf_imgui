//
// Created by petr on 6/12/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ICONS_H
#define PF_IMGUI_SRC_PF_IMGUI_ICONS_H

#include <IconsFontAwesome5.h>
#include <filesystem>

namespace pf::ui::ig {
enum class IconPack { FontAwesome5 };

std::vector<std::filesystem::path> fontFileNamesForIconPack(IconPack iconPack) {
  switch (iconPack) {
    case IconPack::FontAwesome5: return {FONT_ICON_FILE_NAME_FAR, FONT_ICON_FILE_NAME_FAS};
  }
  return {};
}

struct IconFontConfig {
  ImFontConfig config;
  std::vector<ImWchar> iconRange;
};

IconFontConfig fontConfigForIconPack(IconPack iconPack) {
  switch (iconPack) {
    case IconPack::FontAwesome5: {
      auto icons_config = ImFontConfig{};
      icons_config.MergeMode = true;
      icons_config.PixelSnapH = true;
      return {icons_config, {ICON_MIN_FA, ICON_MAX_FA, 0}};
    }
  }
  return {};
}
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ICONS_H
