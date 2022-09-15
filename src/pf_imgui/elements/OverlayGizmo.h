/**
 * @file OverlayGizmo.h
 * @brief Gizmos overlaying entire viewport or a window.
 * @author Petr Flajšingr
 * @date 22.5.22
 */

#ifndef PF_IMGUI_ELEMENTS_OVERLAYGIZMO_H
#define PF_IMGUI_ELEMENTS_OVERLAYGIZMO_H

#include "details/OverlayGizmoBase.h"
#include <pf_common/Explicit.h>
#include <pf_imgui/dialogs/Window.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {

/**
 * @brief Gizmo overlaying the main viewport.
 * @attention Only one gizmo can be used within the app at the same time (this applies to ViewportOverlayGizmo and AreaOverlayGizmo)
 */
class ViewportOverlayGizmo : public Renderable, public OverlayGizmoBase {
 public:
  /**
   * @brief Construction args for ViewportOverlayGizmo
   * // TODO: strong types for each projection type instead of bool flag
   */
  struct Config {
    using Parent = ViewportOverlayGizmo;
    Explicit<std::string_view> name;         /*!< Unique name of the element */
    Explicit<glm::mat4> value;               /*!< Initial value */
    Explicit<ViewportGizmoMode> mode;        /*!< Gizmo mode */
    Explicit<ViewportGizmoSpace> space;      /*!< Gizmo space */
    Explicit<glm::mat4> viewMatrix;          /*!< Camera view matrix */
    Explicit<glm::mat4> projectionMatrix;    /*!< Camera projection matrix */
    Explicit<bool> isProjectionOrthographic; /*!< Set true if projectionMatrix is ortho */
  };
  /**
   * Construct ViewportOverlayGizmo.
   * @param config construction args @see ViewportOverlayGizmo::Config
   */
  explicit ViewportOverlayGizmo(Config &&config);
  /**
   * Construct ViewportOverlayGizmo.
   * @param name unique name of the element
   * @param initialValue initial value
   * @param gizmoMode gizmo mode
   * @param spaceType gizmo space
   * @param initialView camera view matrix
   * @param initialProjection camera projection matrix
   * @param isProjectionOrthographic set true if projectionMatrix is ortho
   */
  ViewportOverlayGizmo(std::string_view name, const glm::mat4 &initialValue, ViewportGizmoMode gizmoMode,
                       ViewportGizmoSpace spaceType, const glm::mat4 &initialView, const glm::mat4 &initialProjection,
                       bool isProjectionOrthographic);

 protected:
  void renderImpl() override;
};

/**
 * @brief Gizmo overlaying its window/layout.
 * @attention Only one gizmo can be used within the app at the same time (this applies to ViewportOverlayGizmo and AreaOverlayGizmo)
 */
class AreaOverlayGizmo : public ElementWithID, public OverlayGizmoBase {
 public:
  /**
   * @brief Construction args for AreaOverlayGizmo
   */
  struct Config {
    using Parent = AreaOverlayGizmo;
    Explicit<std::string_view> name;         /*!< Unique name of the element */
    Window &owningWindow;                    /*!< Parent window of the element */
    Explicit<glm::mat4> value;               /*!< Initial value */
    Explicit<ViewportGizmoMode> mode;        /*!< Gizmo mode */
    Explicit<ViewportGizmoSpace> space;      /*!< Gizmo space */
    Explicit<glm::mat4> viewMatrix;          /*!< Camera view matrix */
    Explicit<glm::mat4> projectionMatrix;    /*!< Camera projection matrix */
    Explicit<bool> isProjectionOrthographic; /*!< Set true if projectionMatrix is ortho */
  };
  /**
   * Construct AreaOverlayGizmo.
   * @param config construction args @see AreaOverlayGizmo::Config
   */
  explicit AreaOverlayGizmo(Config &&config);
  /**
   * Construct AreaOverlayGizmo.
   * @param name unique name of the element
   * @param value initial value
   * @param mode gizmo mode
   * @param space gizmo space
   * @param view camera view matrix
   * @param projection camera projection matrix
   * @param isProjectionOrthographic set true if projectionMatrix is ortho
   */
  AreaOverlayGizmo(std::string_view name, Window &owningWindow, const glm::mat4 &value, ViewportGizmoMode mode,
                   ViewportGizmoSpace space, const glm::mat4 &view, const glm::mat4 &projection,
                   bool isProjectionOrthographic);

 protected:
  void renderImpl() override;

 private:
  Window &parentWindow;
  std::optional<bool> wasWindowMovable{};
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_OVERLAYGIZMO_H