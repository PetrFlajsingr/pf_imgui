/**
 * @file BackgroundDockingArea.h
 * @brief A special type of window, which acts as docking area for the whole viewport.
 * @author Petr Flajšingr
 * @date 14.4.22
 */

#ifndef PF_IMGUI_DIALOGS_BACKGROUNDDOCKINGAREA_H
#define PF_IMGUI_DIALOGS_BACKGROUNDDOCKINGAREA_H

#include <pf_imgui/elements/DockSpace.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {

/**
 * @brief A special type of window, which acts as docking area for the whole viewport. It is always in background/
 */
class BackgroundDockingArea : public Renderable, private Resizable {
 public:
  /**
   * Construct BackgroundDockingArea.
   * @param name unique name of the element
   */
  explicit BackgroundDockingArea(const std::string &name);

  /**
   * Get inner DockSpace. Can be used for building.
   * @return inner DockSpace
   */
  [[nodiscard]] DockSpace &getDockSpace();

 protected:
  void renderImpl() override;

 private:
  DockSpace dockSpace;
  constexpr static auto flags =
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_DIALOGS_BACKGROUNDDOCKINGAREA_H
