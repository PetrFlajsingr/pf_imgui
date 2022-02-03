/**
 * @file ItemElement.h
 * @brief Base class for elements on the lowest level.
 * @author Petr Flajšingr
 * @date 23.1.21
 */

#ifndef PF_IMGUI_INTERFACE_ITEMELEMENT_H
#define PF_IMGUI_INTERFACE_ITEMELEMENT_H

#include "Element.h"
#include "Focusable.h"
#include "Hoverable.h"
#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/elements/Tooltip.h>
#include <string>

namespace pf::ui::ig {
// TODO: move position struct somewhere else and use it here for mouse pos
/**
 * @brief Interface for elements, which are 'leaves' in the UI tree.
 *
 * Provides hover and focus changes. Adds an option to add a tooltip, which is shown when the item is hovered.
 */
class PF_IMGUI_EXPORT ItemElement : public Element, public Focusable, public Hoverable {
 public:
  /**
   * Construct ItemElement with the given ID.
   * @param elementName ID
   */
  explicit ItemElement(const std::string &elementName);

  ItemElement(ItemElement &&other) noexcept;
  ItemElement &operator=(ItemElement &&other) noexcept;

  /**
   * Render this object while also checking for its hover and focus state.
   * @warning DO NOT OVERRIDE UNLESS YOU WANT TO LOSE SOME FUNCTIONALITY
   * @details If you override this function you should base the new implementation on this one, so that no
   * expected functionality is lost.
   */
  void render() override;

  /**
   * Check if this item has an active tooltip.
   * @return true if the tooltip is active, false otherwise
   */
  [[nodiscard]] bool hasTooltip() const;
  /**
   * Get the tooltip if it exists.
   * @return tooltip of this item
   *
   * @throws StacktraceException if the tooltip doesn't exist
   */
  [[nodiscard]] Tooltip &getTooltip();
  /**
   * Removes tooltip if it is assigned. If the tooltip doesn't exist, nothing happens.
   */
  void removeTooltip();
  /**
   * Create a simple tooltip with provided text.
   * @param text text to be shown on tooltip
   */
  void setTooltip(std::string_view text);
  /**
   * Assign a tooltip created elsewhere.
   * @param newTooltip new tooltip
   */
  void setTooltip(std::unique_ptr<Tooltip> &&newTooltip);
  /**
   * Create an instance of tooltip to be filled with element by the user.
   * @return reference to the newly created tooltip
   */
  [[nodiscard]] Tooltip &createTooltip();

  /**
   * Check if this item has an active tooltip.
   * @return true if the tooltip is active, false otherwise
   */
  [[nodiscard]] bool hasPopupMenu() const;
  /**
   * Get the tooltip if it exists.
   * @return tooltip of this item
   *
   * @throws StacktraceException if the tooltip doesn't exist
   */
  [[nodiscard]] PopupMenu &getPopupMenu();
  /**
   * Create an instance of popup menu to be filled with elements by the user.
   * @return reference to the newly created PopupMenu
   */
  [[nodiscard]] PopupMenu &createPopupMenu();

  /**
   * Add a listener to mouse position. Returned value is distance from upper left corner of the element area.
   * @param listener
   * @return Subscription to allow for listener removal
   */
  Subscription addMousePositionListener(std::invocable<ImVec2> auto &&listener) {
    return mousePositionObservable.addListener(std::forward<decltype(listener)>(listener));
  }

  /**
   * Set focus state and keyboard focus for this item.
   */
  void setFocus() override;

 private:
  std::unique_ptr<Tooltip> tooltip = nullptr;
  std::unique_ptr<PopupMenu> popupMenu = nullptr;

  ImVec2 lastMousePosition{-1, -1};
  Observable_impl<ImVec2> mousePositionObservable;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_ITEMELEMENT_H
