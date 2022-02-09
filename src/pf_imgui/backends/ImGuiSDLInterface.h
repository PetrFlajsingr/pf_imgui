//
// Created by Petr on 26.10.2021.
//

#ifndef IMGUI_SW_RENDERER__IMGUISDLINTERFACE_H
#define IMGUI_SW_RENDERER__IMGUISDLINTERFACE_H

#include <pf_imgui/ImGuiInterface.h>
#include <pf_imgui/backends/impl/imgui_impl_sdl.h>
#include <pf_imgui/backends/impl/imgui_impl_sdlrenderer.h>

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

 protected:
  void newFrame_impl() override;
  void renderDrawData_impl(ImDrawData *drawData) override;
};

}  // namespace pf::ui::ig

#endif  //IMGUI_SW_RENDERER__IMGUISDLINTERFACE_H
