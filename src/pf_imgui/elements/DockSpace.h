/*
 * @file DockSpace.h
 * @brief A space for docking windows.
 * @author Petr Flajšingr
 * @date 27.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_DIALOGS_DOCKSPACE_H
#define PF_IMGUI_SRC_PF_IMGUI_DIALOGS_DOCKSPACE_H

#include <imgui.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>

namespace pf::ui::ig {
enum class DockType {
  DisableCentralNode = 1 << 2,
  DisableAreaSplit = 1 << 4,
  DisableAreaResize = 1 << 5,
  AutoHideTabBar = 1 << 6
};
// TODO: somehow keep this alive when the parent is collapsed/invisible -
// TODO: add dock builder impl
/**
 * @brief An area to which dockable windows can be docked.
 * @warning DockSpace HAS TO BE RENDERED AS SOON AS POSSIBLE OTHERWISE YOU WON'T BE ABLE TO DOCK ANYTHING
 */
class PF_IMGUI_EXPORT DockSpace
    : public Element,
      public Resizable,
      public ColorCustomizable<style::ColorOf::DockingPreview, style::ColorOf::DockingBackground> {
 public:
  using Id = ImGuiID;
  struct Config {
    using Parent = DockSpace;
    std::string_view name;
    Size size = Size::Auto();
    Flags<DockType> flags{};
  };
  explicit DockSpace(Config &&config);
  /**
   * Construct DockSpace
   * @param name ID of the element
   * @param s size of the area
   * @param dockFlags flags
   */
  explicit DockSpace(const std::string &name, Size s = Size::Auto(),
                     const Flags<DockType> &dockFlags = Flags<DockType>{});

  void render() override;

  /**
   * @attention Call this only after the first render.
   * @return id to be used for docking windows into the area
   */
  [[nodiscard]] Id getDockId() const;

 protected:
  void renderImpl() override;

 private:
  ImGuiDockNodeFlags flags;
  Id id{};
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SRC_PF_IMGUI_DIALOGS_DOCKSPACE_H
