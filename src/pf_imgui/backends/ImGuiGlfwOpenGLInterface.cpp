//
// Created by xflajs00 on 22.10.2021.
//

#include "ImGuiGlfwOpenGLInterface.h"
#include "impl/imgui_impl_glfw.h"
#include "impl/imgui_impl_opengl3.h"

pf::ui::ig::ImGuiGlfwOpenGLInterface::ImGuiGlfwOpenGLInterface(ImGuiGlfwOpenGLConfig config)
    : ImGuiInterface(config.flags, config.config, config.enableMultiViewport, config.pathToIconFolder,
                     config.enabledIconPacks, config.defaultFontSize) {
  ImGui_ImplGlfw_InitForOpenGL(config.windowHandle, true);
  ImGui_ImplOpenGL3_Init();
  updateFonts();
}

pf::ui::ig::ImGuiGlfwOpenGLInterface::~ImGuiGlfwOpenGLInterface() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void pf::ui::ig::ImGuiGlfwOpenGLInterface::updateFonts() {
  // no need to implement this for OpenGL
}

void pf::ui::ig::ImGuiGlfwOpenGLInterface::render() {
  if (shouldUpdateFontAtlas) {
    shouldUpdateFontAtlas = false;
    updateFonts();
  }
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  RAII endFrameRAII{[&] {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
