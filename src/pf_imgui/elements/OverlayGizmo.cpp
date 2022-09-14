//
// Created by xflajs00 on 22.05.2022.
//

#include "OverlayGizmo.h"

namespace pf::ui::ig {
ViewportOverlayGizmo::ViewportOverlayGizmo(ViewportOverlayGizmo::Config &&config)
    : ViewportOverlayGizmo(std::string{config.name.value}, config.value, config.mode, config.space, config.viewMatrix,
                           config.projectionMatrix, config.isProjectionOrthographic) {}

ViewportOverlayGizmo::ViewportOverlayGizmo(const std::string &elementName, const glm::mat4 &initialValue,
                                           ViewportGizmoMode gizmoMode, ViewportGizmoSpace spaceType,
                                           const glm::mat4 &initialView, const glm::mat4 &initialProjection,
                                           bool isProjectionOrthographic)
    : Renderable(elementName),
      OverlayGizmoBase(initialValue, gizmoMode, spaceType, initialView, initialProjection, isProjectionOrthographic) {}

void ViewportOverlayGizmo::renderImpl() {
  ImGuizmo::SetOrthographic(projectionOrthographic);

  auto &io = ImGui::GetIO();
  ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
  drawImpl(*enabled);
}

AreaOverlayGizmo::AreaOverlayGizmo(AreaOverlayGizmo::Config &&config)
    : AreaOverlayGizmo(std::string{config.name.value}, config.owningWindow, config.value, config.mode, config.space,
                       config.viewMatrix, config.projectionMatrix, config.isProjectionOrthographic) {}

AreaOverlayGizmo::AreaOverlayGizmo(const std::string &elementName, Window &owningWindow, const glm::mat4 &initialValue,
                                   ViewportGizmoMode gizmoMode, ViewportGizmoSpace gizmoSpace,
                                   const glm::mat4 &initialView, const glm::mat4 &initialProjection,
                                   bool isProjectionOrthographic)
    : ElementWithID(elementName),
      OverlayGizmoBase(initialValue, gizmoMode, gizmoSpace, initialView, initialProjection, isProjectionOrthographic),
      parentWindow(owningWindow) {}

void AreaOverlayGizmo::renderImpl() {
  ImGuizmo::SetOrthographic(projectionOrthographic);

  ImGuizmo::SetDrawlist();
  const auto windowWidth = static_cast<float>(ImGui::GetWindowWidth());
  const auto windowHeight = static_cast<float>(ImGui::GetWindowHeight());
  ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
  drawImpl(*enabled);
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