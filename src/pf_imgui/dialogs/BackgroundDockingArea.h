/**
 * @file BackgroundDockingArea.h
 * @brief A special type of window, which acts as docking area for the whole viewport.
 * @author Petr Flajšingr
 * @date 14.4.22
 */

#ifndef PF_IMGUI_DIALOGS_BACKGROUNDDOCKINGAREA_H
#define PF_IMGUI_DIALOGS_BACKGROUNDDOCKINGAREA_H

#include <pf_imgui/_export.h>
#include <pf_imgui/elements/DockSpace.h>
#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {

/**
 * @brief A special type of window, which acts as docking area for the whole main viewport. It is always in the background.
 */
class PF_IMGUI_EXPORT BackgroundDockingArea : public Renderable {
  friend class ImGuiInterface;

 public:
  /**
   * Construct BackgroundDockingArea.
   * @param elementName unique name of the element
   */
  explicit BackgroundDockingArea(std::string_view elementName);

  /**
   * Get inner DockSpace. Can be used for building.
   * @return inner DockSpace
   */
  [[nodiscard]] DockSpace &getDockSpace();

  /** Size automatically computed to fit the viewport */
  ReadOnlyProperty<Size> size;

 protected:
  void renderImpl() override;

 private:
  ImVec2 leftTopMargin = ImVec2{0, 0};
  ImVec2 bottomRightMargin = ImVec2{0, 0};
  DockSpace dockSpace;
  constexpr static auto flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_DIALOGS_BACKGROUNDDOCKINGAREA_H
