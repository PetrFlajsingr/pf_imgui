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

  ColorPalette<ColorOf::DockingPreview, ColorOf::DockingBackground> color;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_DIALOGS_BACKGROUNDDOCKINGAREA_H
