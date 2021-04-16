//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_INTERFACE_ITEMELEMENT_H
#define PF_IMGUI_INTERFACE_ITEMELEMENT_H

#include "Element.h"
#include "Focusable.h"
#include "Hoverable.h"
#include "src/pf_imgui/elements/Tooltip.h"
#include <memory>
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

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
   * Create an instance of tooltip to be filled with element by the user.
   * @return reference to the newly created tooltip
   */
  [[nodiscard]] Tooltip &createTooltip();

  /**
   * Set focus state and keyboard focus for this item.
   */
  void setFocus() override;

 private:
  std::unique_ptr<Tooltip> tooltip = nullptr;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_ITEMELEMENT_H
