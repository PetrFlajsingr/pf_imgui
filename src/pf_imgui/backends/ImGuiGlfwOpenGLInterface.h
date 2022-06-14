//
// Created by xflajs00 on 22.10.2021.
//

#ifndef PF_IMGUI_BACKENDS_IMGUIGLFWOPENGLINTERFACE_H
#define PF_IMGUI_BACKENDS_IMGUIGLFWOPENGLINTERFACE_H

#include <GLFW/glfw3.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {

struct ImGuiGlfwOpenGLConfig {
  ImGuiConfig imgui;
  GLFWwindow *windowHandle;
};

class ImGuiGlfwOpenGLInterface final : public ImGuiInterface {
 public:
  explicit ImGuiGlfwOpenGLInterface(ImGuiGlfwOpenGLConfig config);
  ~ImGuiGlfwOpenGLInterface() override;

  void updateFonts() override;

  void processInput() override;

 protected:
  void newFrame_impl() override;
  void renderDrawData_impl(ImDrawData *drawData) override;
  void updateMultiViewport() override;
};

}  // namespace pf::ui::ig
#endif  //PF_IMGUI_BACKENDS_IMGUIGLFWOPENGLINTERFACE_H
