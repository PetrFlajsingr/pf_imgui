//
// Created by Petr on 26.10.2021.
//

#ifndef IMGUI_SW_RENDERER__IMGUISDLINTERFACE_H
#define IMGUI_SW_RENDERER__IMGUISDLINTERFACE_H

#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {

struct ImGuiSDLConfig {
  SDL_Window *windowHandle;
  SDL_Renderer *renderer;
  ImGuiConfigFlags flags = {};
  bool enableMultiViewport = false;
  toml::table config;
  std::filesystem::path pathToIconFolder;
  Flags<IconPack> enabledIconPacks = Flags<IconPack>{};
  float defaultFontSize = 13.f;
};

class ImGuiSDLInterface : public ImGuiInterface {
 public:
  ImGuiSDLInterface(const ImGuiSDLConfig &config);

  virtual ~ImGuiSDLInterface();

  void updateFonts() override;

  void render() override;
};

}// namespace pf::ui::ig

#endif//IMGUI_SW_RENDERER__IMGUISDLINTERFACE_H
