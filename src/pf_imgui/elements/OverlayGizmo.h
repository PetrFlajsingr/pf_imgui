/**
 * @file OverlayGizmo.h
 * @brief Gizmos overlaying entire viewport or a window.
 * @author Petr Flajšingr
 * @date 22.5.22
 */

#ifndef PF_IMGUI_ELEMENTS_OVERLAYGIZMO_H
#define PF_IMGUI_ELEMENTS_OVERLAYGIZMO_H

#include "details/OverlayGizmoBase.h"
#include <pf_imgui/dialogs/Window.h>
#include <pf_imgui/interface/Element.h>
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
   */
  struct Config {
    using Parent = ViewportOverlayGizmo;
    std::string_view name;         /*!< Unique name of the element */
    glm::mat4 value;               /*!< Initial value */
    ViewportGizmoMode mode;        /*!< Gizmo mode */
    ViewportGizmoSpace space;      /*!< Gizmo space */
    glm::mat4 viewMatrix;          /*!< Camera view matrix */
    glm::mat4 projectionMatrix;    /*!< Camera projection matrix */
    bool isProjectionOrthographic; /*!< Set true if projectionMatrix is ortho */
  };
  /**
   * Construct ViewportOverlayGizmo.
   * @param config construction args @see ViewportOverlayGizmo::Config
   */
  explicit ViewportOverlayGizmo(Config &&config);
  /**
   * Construct ViewportOverlayGizmo.
   * @param name unique name of the element
   * @param value initial value
   * @param mode gizmo mode
   * @param space gizmo space
   * @param view camera view matrix
   * @param projection camera projection matrix
   * @param isProjectionOrthographic set true if projectionMatrix is ortho
   */
  ViewportOverlayGizmo(const std::string &name, const glm::mat4 &value, ViewportGizmoMode mode,
                       ViewportGizmoSpace space, const glm::mat4 &view, const glm::mat4 &projection,
                       bool isProjectionOrthographic);

 protected:
  void renderImpl() override;
};

/**
 * @brief Gizmo overlaying its window/layout.
 * @attention Only one gizmo can be used within the app at the same time (this applies to ViewportOverlayGizmo and AreaOverlayGizmo)
 */
class AreaOverlayGizmo : public Element, public OverlayGizmoBase {
 public:
  /**
   * @brief Construction args for AreaOverlayGizmo
   */
  struct Config {
    using Parent = AreaOverlayGizmo;
    std::string_view name;         /*!< Unique name of the element */
    Window &owningWindow;          /*!< Parent window of the element */
    glm::mat4 value;               /*!< Initial value */
    ViewportGizmoMode mode;        /*!< Gizmo mode */
    ViewportGizmoSpace space;      /*!< Gizmo space */
    glm::mat4 viewMatrix;          /*!< Camera view matrix */
    glm::mat4 projectionMatrix;    /*!< Camera projection matrix */
    bool isProjectionOrthographic; /*!< Set true if projectionMatrix is ortho */
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
  AreaOverlayGizmo(const std::string &name, Window &owningWindow, const glm::mat4 &value, ViewportGizmoMode mode,
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