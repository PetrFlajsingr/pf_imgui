/*
 * @file DockSpace.h
 * @brief A space for docking windows.
 * @author Petr Flajšingr
 * @date 27.6.21
 */

#ifndef PF_IMGUI_ELEMENTS_DOCKSPACE_H
#define PF_IMGUI_ELEMENTS_DOCKSPACE_H

#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/style/ColorPalette.h>

namespace pf::ui::ig {

enum class DockType { DisableCentralNode = 1 << 2, DisableAreaSplit = 1 << 4, DisableAreaResize = 1 << 5, AutoHideTabBar = 1 << 6 };
// TODO: somehow keep this alive when the parent is collapsed/invisible -
/**
 * @brief An area to which dockable windows can be docked.
 * @warning DockSpace HAS TO BE RENDERED AS SOON AS POSSIBLE OTHERWISE YOU WON'T BE ABLE TO DOCK ANYTHING
 */
class PF_IMGUI_EXPORT DockSpace : public ElementWithID {
 public:
  using Id = ImGuiID;
  /**
   * @brief Struct for construction of DockSpace.
   */
  struct Config {
    using Parent = DockSpace;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Size size = Size::Auto();        /*!< Size of the element */
    Flags<DockType> flags{};         /*!< Flags to modify functionality */
  };
  /**
   * Construct DockSpace
   * @param config construction args @see DockSpace::Config
   */
  explicit DockSpace(Config &&config);
  /**
   * Construct DockSpace
   * @param name ID of the element
   * @param s size of the area
   * @param dockFlags flags
   */
  explicit DockSpace(std::string_view name, Size s = Size::Auto(), Flags<DockType> dockFlags = Flags<DockType>{});

  [[nodiscard]] bool isInitialised() const;

  void render() override;

  ColorPalette<ColorOf::DockingPreview, ColorOf::DockingBackground> color;

  Property<Size> size;

 protected:
  void renderImpl() override;

 private:
  bool firstFrame = true;
  ImGuiDockNodeFlags flags;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_DOCKSPACE_H
