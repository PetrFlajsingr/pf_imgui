//
// Created by Petr on 26.10.2021.
//

#include "ImGuiSDLInterface.h"
#include <SDL.h>

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

void ImGuiSDLInterface::processInput() {
  SDL_PumpEvents();
  constexpr static auto MAX_EVENTS = 10;
  std::array<SDL_Event, MAX_EVENTS> events;
  const auto eventCount =
      SDL_PeepEvents(events.data(), events.size(), SDL_eventaction::SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
  for (int i = 0; i < eventCount; ++i) { ImGui_ImplSDL2_ProcessEvent(&events[i]); }
}

void ImGuiSDLInterface::newFrame_impl() {
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();
}

void ImGuiSDLInterface::renderDrawData_impl(ImDrawData *drawData) { ImGui_ImplSDLRenderer_RenderDrawData(drawData); }

}  // namespace pf::ui::ig
