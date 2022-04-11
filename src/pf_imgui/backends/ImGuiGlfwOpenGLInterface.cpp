//
// Created by xflajs00 on 22.10.2021.
//

#include "ImGuiGlfwOpenGLInterface.h"
#include "impl/imgui_impl_glfw.h"
#include "impl/imgui_impl_opengl3.h"

namespace pf::ui::ig {

ImGuiGlfwOpenGLInterface::ImGuiGlfwOpenGLInterface(ImGuiGlfwOpenGLConfig config)
    : ImGuiInterface(std::move(config.imgui)) {
  ImGui_ImplGlfw_InitForOpenGL(config.windowHandle, true);
  ImGui_ImplOpenGL3_Init();
  updateFonts();
}

ImGuiGlfwOpenGLInterface::~ImGuiGlfwOpenGLInterface() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
}

void ImGuiGlfwOpenGLInterface::updateFonts() {
  // no need to implement this for OpenGL
}

void ImGuiGlfwOpenGLInterface::processInput() {
  // done via callbacks
}

void ImGuiGlfwOpenGLInterface::newFrame_impl() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
}
void ImGuiGlfwOpenGLInterface::renderDrawData_impl(ImDrawData *drawData) { ImGui_ImplOpenGL3_RenderDrawData(drawData); }

}  // namespace pf::ui::ig
