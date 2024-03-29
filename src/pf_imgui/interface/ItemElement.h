/**
 * @file ItemElement.h
 * @brief Base class for elements on the lowest level.
 * @author Petr Flajšingr
 * @date 23.1.21
 */

#ifndef PF_IMGUI_INTERFACE_ITEMELEMENT_H
#define PF_IMGUI_INTERFACE_ITEMELEMENT_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Position.h>
#include <pf_imgui/fwd.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Interface for elements, which are 'leaves' in the UI tree.
 *
 * Provides hover and focus changes. Adds an option to add a tooltip, which is shown when the item is hovered.
 */
class PF_IMGUI_EXPORT ItemElement : public ElementWithID {
 public:
  /**
   * Construct ItemElement with the given ID.
   * @param elementName ID
   */
  explicit ItemElement(std::string_view elementName);
  ~ItemElement() override;

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
   * Create an instance of tooltip to be filled with element by the user or get an existing one.
   * @return reference to the created tooltip
   */
  [[nodiscard]] Tooltip &createOrGetTooltip();
  /**
   * Check if this item has an active tooltip.
   * @return true if the tooltip is active, false otherwise
   */
  [[nodiscard]] bool hasTooltip() const;
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
   * Create an instance of popup menu to be filled with elements by the user or get an existing one.
   * @return reference to the newly created PopupMenu
   */
  [[nodiscard]] PopupMenu &createOrGetPopupMenu();
  /**
   * Check if this item has an active tooltip.
   * @return true if the tooltip is active, false otherwise
   */
  [[nodiscard]] bool hasPopupMenu() const;
  /**
   * Removes popup menu if it is assigned. If it doesn't exist, nothing happens.
   */
  void removePopupMenu();

  ReadOnlyProperty<bool> hovered;
  ReadOnlyProperty<bool> focused;
  ReadOnlyProperty<Position> mousePosition;

 private:
  std::unique_ptr<Tooltip> tooltip;
  std::unique_ptr<PopupMenu> popupMenu;

  ImVec2 lastMousePosition{-1, -1};
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_ITEMELEMENT_H
