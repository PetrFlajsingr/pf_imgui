//
// Created by xflajs00 on 22.10.2021.
//

#ifndef OPENGL_TEMPLATE_CMAKE_BUILD_DEBUG__DEPS_PF_IMGUI_SRC_SRC_PF_IMGUI_BACKENDS_IMGUIGLFWOPENGLINTERFACE_H
#define OPENGL_TEMPLATE_CMAKE_BUILD_DEBUG__DEPS_PF_IMGUI_SRC_SRC_PF_IMGUI_BACKENDS_IMGUIGLFWOPENGLINTERFACE_H

#include <GLFW/glfw3.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {

struct ImGuiGlfwOpenGLConfig {
  GLFWwindow *windowHandle;
  ImGuiConfigFlags flags = {};
  bool enableMultiViewport = false;
  toml::table config;
  std::filesystem::path pathToIconFolder;
  Flags<IconPack> enabledIconPacks = Flags<IconPack>{};
  float defaultFontSize = 13.f;
};

class ImGuiGlfwOpenGLInterface : public ImGuiInterface {
 public:
  ImGuiGlfwOpenGLInterface(ImGuiGlfwOpenGLConfig config);
  virtual ~ImGuiGlfwOpenGLInterface();

  void updateFonts() override;
  void render() override;

 protected:
  void renderImpl() override;

 private:
};

}// namespace pf::ui::ig
#endif//OPENGL_TEMPLATE_CMAKE_BUILD_DEBUG__DEPS_PF_IMGUI_SRC_SRC_PF_IMGUI_BACKENDS_IMGUIGLFWOPENGLINTERFACE_H