//
// Created by xflajs00 on 22.05.2022.
//

#ifndef PF_IMGUI_ELEMENTS_DETAILS_OVERLAYGIZMO_BASE_H
#define PF_IMGUI_ELEMENTS_DETAILS_OVERLAYGIZMO_BASE_H

#include <ImGuizmo.h>
#include <glm/matrix.hpp>
#include <optional>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/reactive/Observable.h>

namespace pf::ui::ig {

enum class ViewportGizmoMode : int {
  TranslateX = ImGuizmo::TRANSLATE_X,
  TranslateY = ImGuizmo::TRANSLATE_Y,
  TranslateZ = ImGuizmo::TRANSLATE_Z,
  RotateX = ImGuizmo::ROTATE_X,
  RotateY = ImGuizmo::ROTATE_Y,
  RotateZ = ImGuizmo::ROTATE_Z,
  RotateScreen = ImGuizmo::ROTATE_SCREEN,
  ScaleX = ImGuizmo::SCALE_X,
  ScaleY = ImGuizmo::SCALE_Y,
  ScaleZ = ImGuizmo::SCALE_Z,
  //Bounds = ImGuizmo::BOUNDS        ,
  ScaleXU = ImGuizmo::SCALE_XU,
  ScaleYU = ImGuizmo::SCALE_YU,
  ScaleZU = ImGuizmo::SCALE_ZU,

  Translate = ImGuizmo::TRANSLATE,
  Rotate = ImGuizmo::ROTATE,
  Scale = ImGuizmo::SCALE,
  ScaleU = ImGuizmo::SCALEU,
  Universal = ImGuizmo::UNIVERSAL
};

enum class ViewportGizmoSpace { Local = ImGuizmo::LOCAL, World = ImGuizmo::WORLD };

class OverlayGizmoBase : public ValueContainer<glm::mat4> {
 public:
  OverlayGizmoBase(const glm::mat4 &initialValue, ViewportGizmoMode gizmoMode, ViewportGizmoSpace gizmoSpace,
                   const glm::mat4 &initialView, const glm::mat4 &initialProjection, bool isProjectionOrthographic);

  [[nodiscard]] ViewportGizmoMode getMode() const;
  void setMode(ViewportGizmoMode newMode);

  [[nodiscard]] ViewportGizmoSpace getSpace() const;
  void setSpace(ViewportGizmoSpace newSpace);

  [[nodiscard]] const glm::mat4 &getView() const;
  void setView(const glm::mat4 &newView);

  [[nodiscard]] const glm::mat4 &getProjection() const;
  void setProjection(const glm::mat4 &newProjection, bool isOrthographic);

  void setSnapValues(float translate, float rotate, float scale);

  void setValue(const glm::mat4 &newValue) override;
  const glm::mat4 &getValue() const override;

 protected:
  Subscription addValueListenerImpl(std::function<void(glm::mat4)> listener) override;

 public:
  ObservableProperty<OverlayGizmoBase, glm::mat4> transform;
  /** Gizmo is actively being interacted with */
  Observable<bool> inUse;

 protected:
  void drawImpl(bool isEnabled);

  ViewportGizmoMode mode;
  ViewportGizmoSpace space;

  std::optional<glm::vec3> snapValues = std::nullopt;

  glm::mat4 view;
  glm::mat4 projection;
  bool projectionOrthographic;

  bool wasUsing = false;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_DETAILS_OVERLAYGIZMO_BASE_H