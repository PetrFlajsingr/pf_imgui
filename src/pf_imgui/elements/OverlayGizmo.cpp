//
// Created by xflajs00 on 22.05.2022.
//

#include "OverlayGizmo.h"

namespace pf::ui::ig {
ViewportOverlayGizmo::ViewportOverlayGizmo(ViewportOverlayGizmo::Config &&config)
    : ViewportOverlayGizmo(std::string{config.name.value}, config.value, config.mode, config.space, config.viewMatrix,
                           config.projectionMatrix, config.isProjectionOrthographic) {}

ViewportOverlayGizmo::ViewportOverlayGizmo(const std::string &name, const glm::mat4 &value, ViewportGizmoMode mode,
                                           ViewportGizmoSpace space, const glm::mat4 &view, const glm::mat4 &projection,
                                           bool isProjectionOrthographic)
    : Renderable(name), OverlayGizmoBase(value, mode, space, view, projection, isProjectionOrthographic) {}
void ViewportOverlayGizmo::renderImpl() {
  ImGuizmo::SetOrthographic(projectionOrthographic);

  auto &io = ImGui::GetIO();
  ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
  drawImpl(getEnabled() == pf::Enabled::Yes);
}

AreaOverlayGizmo::AreaOverlayGizmo(AreaOverlayGizmo::Config &&config)
    : AreaOverlayGizmo(std::string{config.name.value}, config.owningWindow, config.value, config.mode, config.space,
                       config.viewMatrix, config.projectionMatrix, config.isProjectionOrthographic) {}

AreaOverlayGizmo::AreaOverlayGizmo(const std::string &name, Window &owningWindow, const glm::mat4 &value,
                                   ViewportGizmoMode mode, ViewportGizmoSpace space, const glm::mat4 &view,
                                   const glm::mat4 &projection, bool isProjectionOrthographic)
    : ElementWithID(name), OverlayGizmoBase(value, mode, space, view, projection, isProjectionOrthographic),
      parentWindow(owningWindow) {}

void AreaOverlayGizmo::renderImpl() {
  ImGuizmo::SetOrthographic(projectionOrthographic);

  ImGuizmo::SetDrawlist();
  const auto windowWidth = static_cast<float>(ImGui::GetWindowWidth());
  const auto windowHeight = static_cast<float>(ImGui::GetWindowHeight());
  ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
  drawImpl(getEnabled() == pf::Enabled::Yes);
  if (wasUsing) {
    if (!wasWindowMovable.has_value()) { wasWindowMovable = parentWindow.isUserMovable(); }
    parentWindow.setUserMovable(false);
  } else {
    if (wasWindowMovable.has_value()) {
      parentWindow.setUserMovable(*wasWindowMovable);
      wasWindowMovable.reset();
    }
  }
}

}  // namespace pf::ui::ig