//
// Created by Petr on 26.10.2021.
//

#include "ImGuiSDLInterface.h"

namespace pf::ui::ig {

ImGuiSDLInterface::ImGuiSDLInterface(ImGuiSDLConfig config) : ImGuiInterface(std::move(config.imgui)) {
  ImGui_ImplSDL2_InitForSDLRenderer(config.windowHandle, config.renderer);
  ImGui_ImplSDLRenderer_Init(config.renderer);
  updateFonts();
}

ImGuiSDLInterface::~ImGuiSDLInterface() {
  ImGui_ImplSDL2_Shutdown();
  ImGui_ImplSDLRenderer_Shutdown();
}

void ImGuiSDLInterface::updateFonts() {
  // no need to implement this for SDL
}

void ImGuiSDLInterface::handleSDLEvent(const SDL_Event &event) {
  ImGui_ImplSDL2_ProcessEvent(&event);
}

void ImGuiSDLInterface::newFrame_impl() {
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();
}

void ImGuiSDLInterface::renderDrawData_impl(ImDrawData *drawData) { ImGui_ImplSDLRenderer_RenderDrawData(drawData); }

}  // namespace pf::ui::ig
