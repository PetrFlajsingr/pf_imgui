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
#include <pf_imgui/elements/MenuBars.h>
#include <pf_imgui/interface/Collapsible.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Focusable.h>
#include <pf_imgui/interface/Hoverable.h>
#include <pf_imgui/interface/Positionable.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Main building piece for UI. Persistent element container with plenty of controls.
 *
 * May have a menu bar for additional controls.
 * @todo: fonts
 */
class PF_IMGUI_EXPORT Window : public Renderable,
                               public ElementContainer,
                               public Focusable,
                               public Hoverable,
                               public Collapsible,
                               public Resizable,
                               public Positionable,
                               public Labellable {
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
  const std::optional<Size> &getMinSizeConstraint() const;
  /**
   * Window max size constraints.
   * @return
   */
  const std::optional<Size> &getMaxSizeConstraint() const;
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
  void setCollapsed(bool collapsed) override;
  void setPosition(ImVec2 pos) override;

  /**
   * Check if the Window can be closed with an X button in top right corner.
   * @return
   */
  [[nodiscard]] bool isCloseable() const;
  /**
   * Enable/disable user window closing using an X button in top right corner.
   * @param closeable
   */
  void setCloseable(bool closeable);

  /**
   * Add a listener for close event.
   *
   * This event is triggered when X button is clicked.
   * @param listener to be called on event
   * @return Subscription for listener cancelation
   */
  Subscription addCloseListener(std::invocable auto listener) {
    return closeObservableImpl.template addListener(listener);
  }

  /**
   * Set font for Window and all elements inside - except for those that have their own font.
   * @param fontPtr new font
   */
  void setFont(ImFont *fontPtr);

 protected:
  void renderImpl() override;

 private:
  std::unique_ptr<WindowMenuBar> menuBar = nullptr;
  std::optional<Size> minSizeConstraint = std::nullopt;
  std::optional<Size> maxSizeConstraint = std::nullopt;
  bool closeable = false;
  Observable_impl<> closeObservableImpl;

  bool firstPass = true;

  ImGuiWindowFlags createWindowFlags();
  ImFont *font = nullptr;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_WINDOW_H
