//
// Created by Petr on 26.10.2021.
//

#include "ImGuiSDLInterface.h"

pf::ui::ig::ImGuiSDLInterface::ImGuiSDLInterface(const pf::ui::ig::ImGuiSDLConfig &config)
    : ImGuiInterface(config) {
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
void pf::ui::ig::ImGuiSDLInterface::newFrame_impl() {
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();
}
void pf::ui::ig::ImGuiSDLInterface::renderDrawData_impl(ImDrawData *drawData) {
  ImGui_ImplSDLRenderer_RenderDrawData(drawData);
}