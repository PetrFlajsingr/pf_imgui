/**
 * @file Window.h
 * @brief Dialog for building windows.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_WINDOW_H
#define PF_IMGUI_ELEMENTS_WINDOW_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/DockSpace.h>
#include <pf_imgui/elements/MenuBars.h>
#include <pf_imgui/interface/Closeable.h>
#include <pf_imgui/interface/Collapsible.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Focusable.h>
#include <pf_imgui/interface/Hoverable.h>
#include <pf_imgui/interface/Positionable.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Main building piece for UI. Persistent element container with plenty of controls.
 *
 * May have a menu bar for additional controls.
 *
 * @TODO: ImGui::GetWindowDockId()-> need to have a getter for that and somehow change that to Dock name
 * @TODO: ImGui::IsWindowDocked()
 */
class PF_IMGUI_EXPORT Window : public Renderable,
                               public ElementContainer,
                               public Focusable,
                               public Hoverable,
                               public Collapsible,
                               public Resizable,
                               public Positionable,
                               public Labellable,
                               public Closeable {
 public:
  /**
   * Construct Window.
   * @param name ID of the window
   * @param label title
   * @param allowCollapse
   * @param persistent saving state to disk
   */
  Window(std::string name, std::string label, AllowCollapse allowCollapse = AllowCollapse::No,
         Persistent persistent = Persistent::No);

  /**
   * Construct Window.
   * @param name ID of the window
   * @param label title
   * @param persistent saving state to disk
   */
  Window(std::string name, std::string label, Persistent persistent);

  /**
   * Provides Windows menu bar. If the menu bar doesn't exist new one is created.
   * @return Windows menu bar
   */
  [[nodiscard]] WindowMenuBar &getMenuBar();

  /**
   * Check if the menu bar exists.
   * @return true if menu bar exists, false otherwise
   */
  [[nodiscard]] bool hasMenuBar() const;

  /**
   * Remove menu bar from the window - must be rebuilt when created again, use menuBar.setVisible... for temporary removal.
   */
  void removeMenuBar();

  /**
   * Window min size constraints.
   * @return
   */
  [[nodiscard]] const std::optional<Size> &getMinSizeConstraint() const;

  /**
   * Window max size constraints.
   * @return
   */
  [[nodiscard]] const std::optional<Size> &getMaxSizeConstraint() const;

  /**
   * Min size constraints for window - window can't be smaller.
   * @param sizeConstraints
   */
  void setMinSizeConstraint(const Size &newSizeConstraint);

  /**
   * Max size constraints for window - window can't be bigger.
   * @param sizeConstraints
   */
  void setMaxSizeConstraint(const Size &newSizeConstraint);

  void cancelSizeConstraint();

  void cancelMinSizeConstraint();

  void cancelMaxSizeConstraint();

  void setSize(const Size &size) override;

  void render() override;

  void setFocus() override;

  void setCollapsed(bool collapse) override;

  void setPosition(Position pos) override;

  /**
   * Check if user can resize the window.
   * @return true if resizable
   */
  [[nodiscard]] bool isUserResizable() const;

  /**
   * Enable/disable user resizing the window.
   * @param resizable
   */
  void setUserResizable(bool resizable);

  /**
   * Check if the user can move the window.
   * @return  true if movable
   */
  [[nodiscard]] bool isUserMovable() const;

  /**
   * Enable/disable user moving the window.
   * @param movable
   */
  void setUserMovable(bool movable);

  /**
   * Check if the window auto resizes to fix its content.
   * @return
   */
  [[nodiscard]] bool isAutoResize() const;

  /**
   * Enable/disable window auto resizing to fit its content.
   * @param autoResize
   */
  void setAutoResize(bool autoResize);

  /**
   * Check if horizontal scroll is allowed when content is bigger than the window.
   * @return
   */
  [[nodiscard]] bool isHorizontalScrollEnabled() const;

  /**
   * Enable/disable horizontal scroll when the content is bigger than the window.
   * @param horizontalScroll
   */
  void setHorizontalScrollEnabled(bool horizontalScroll);

  /**
   * Check if a dot is displayed in the top bar of the window.
   * @return
   */
  [[nodiscard]] bool isDisplayDot() const;

  /**
   * Enable/disable displaying a dot in the top bar of the window.
   * @param display
   */
  void setDisplayDot(bool display);

  /**
   *
   * @return true if the window serves as a docking area and can be docked itself
   */
  [[nodiscard]] bool isDockable() const;

  /**
   * If true the window can be used as a dock for other windows or can be docked itself.
   * @param isArea
   */
  void setIsDockable(bool dockable);

  /**
   *
   * @return true if the top bar of the window is being rendered
   */
  [[nodiscard]] bool isTitleBarVisible() const;

  /**
   * Set visibility of top bar of the window.
   * @param visible
   */
  void setTitleBarVisible(bool visible);

  /**
   *
   * @return true if this window is docked in some sort of dockable area
   */
  [[nodiscard]] bool isDocked() const;

  /**
   * Move the window to a dockable area in next frame.
   * @param dockName name of the area
   */
  void moveToDock(DockSpace::Id dockSpaceId);

  /**
   * Set font for Window and all elements inside - except for those that have their own font.
   * @param fontPtr new font
   */
  void setFont(ImFont *fontPtr);

  /**
   *
   * @return true if the window doesn't bring itself on top of others on focus
   */
  [[nodiscard]] bool isStayInBackground() const;
  /**
   * Set if the window stays in background even if it's focused;
   * @param stay
   */
  void setStayInBackground(bool stay);

  void setCollapsible(bool newCollapsible) override;

  std::vector<Renderable *> getRenderables() override;

  // FIXME: remove this and fix up how this is handled in docking
  [[nodiscard]] inline std::string getImGuiName() const { return idLabel; }

 protected:
  void renderImpl() override;

 private:
  void refreshIdLabel();

  std::unique_ptr<WindowMenuBar> menuBar = nullptr;
  std::optional<Size> minSizeConstraint = std::nullopt;
  std::optional<Size> maxSizeConstraint = std::nullopt;

  bool isWindowDocked = false;

  bool firstPass = true;

  ImFont *font = nullptr;

  std::optional<ImGuiID> dockInto = std::nullopt;

  ImGuiWindowFlags flags;

  std::string idLabel{};
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_WINDOW_H
