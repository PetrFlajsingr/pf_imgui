//
// Created by Petr on 26.10.2021.
//

#include "ImGuiSDLInterface.h"

pf::ui::ig::ImGuiSDLInterface::ImGuiSDLInterface(const pf::ui::ig::ImGuiSDLConfig &config)
    : ImGuiInterface(config.flags, config.config, config.enableMultiViewport, config.pathToIconFolder,
                     config.enabledIconPacks, config.defaultFontSize) {
  ImGui_ImplSDL2_InitForSDLRenderer(config.windowHandle);
  ImGui_ImplSDLRenderer_Init(config.renderer);
  updateFonts();
}

pf::ui::ig::ImGuiSDLInterface::~ImGuiSDLInterface() {
  ImGui_ImplSDL2_Shutdown();
  ImGui_ImplSDLRenderer_Shutdown();
}

void pf::ui::ig::ImGuiSDLInterface::updateFonts() {
  // no need to implement this for SDL
}

void pf::ui::ig::ImGuiSDLInterface::render() {
  if (shouldUpdateFontAtlas) {
    shouldUpdateFontAtlas = false;
    updateFonts();
  }
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  RAII endFrameRAII{[&] {
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    if (getIo().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }
  }};
  if (getVisibility() == Visibility::Visible) {
    if (getEnabled() == Enabled::No) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      auto raiiEnabled = pf::RAII([] {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      });
      renderImpl();
    } else {
      renderImpl();
    }
  }
}