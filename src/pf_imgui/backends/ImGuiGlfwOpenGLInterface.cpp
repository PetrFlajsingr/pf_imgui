//
// Created by xflajs00 on 22.10.2021.
//

#include "ImGuiGlfwOpenGLInterface.h"
#include "impl/imgui_impl_glfw.h"
#include "impl/imgui_impl_opengl3.h"

pf::ui::ig::ImGuiGlfwOpenGLInterface::ImGuiGlfwOpenGLInterface(ImGuiGlfwOpenGLConfig config)
    : ImGuiInterface(std::move(config.imgui)) {
  ImGui_ImplGlfw_InitForOpenGL(config.windowHandle, true);
  ImGui_ImplOpenGL3_Init();
  updateFonts();
}

pf::ui::ig::ImGuiGlfwOpenGLInterface::~ImGuiGlfwOpenGLInterface() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
}

void pf::ui::ig::ImGuiGlfwOpenGLInterface::updateFonts() {
  // no need to implement this for OpenGL
}

void pf::ui::ig::ImGuiGlfwOpenGLInterface::newFrame_impl() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
}

void pf::ui::ig::ImGuiGlfwOpenGLInterface::renderDrawData_impl(ImDrawData *drawData) {
  ImGui_ImplOpenGL3_RenderDrawData(drawData);
}
