/**
 * @file Layout.h
 * @brief Base class for layouts.
 * @author Petr Flajšingr
 * @date 25.1.21
 */

#ifndef PF_IMGUI_LAYOUTS_LAYOUT_H
#define PF_IMGUI_LAYOUTS_LAYOUT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/RenderablesContainer.h>
#include <src/pf_imgui/interface/Collapsible.h>
#include <src/pf_imgui/interface/Element.h>
#include <src/pf_imgui/interface/Resizable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

// TODO: collapse support in children
// TODO: flags getter for children
// TODO: delimit ratio
/**
 * @brief Base class for layouts.
 *
 * Provides basic functionality for layouts - border drawing and scrolling.
 */
class PF_IMGUI_EXPORT Layout : public Element, public Collapsible, public RenderablesContainer {
 public:
  /**
   * Construct a layout.
   * @param elementName ID of the element
   * @param allowCollapse allow collapse button
   * @param showBorder when allowed a border is drawn at the edges of the layout area
   * @param persistent when enabled the collapse state of the layout is stored on disk
   */
  explicit Layout(const std::string &elementName, AllowCollapse allowCollapse, ShowBorder showBorder,
                  Persistent persistent);

  /**
   * Check if border is drawn.
   * @return true if border is drawn, false otherwise
   */
  [[nodiscard]] bool isDrawBorder() const;
  /**
   * Set border drawing.
   * @param drawBorder new value
   */
  void setDrawBorder(bool drawBorder);

  /**
   * Check if the layout is scrollable.
   * @return true if layout is scrollable, false otherwise
   * @todo: turn into an interface?
   */
  [[nodiscard]] bool isScrollable() const;
  /**
   * Set layout scrollable
   * @param scrollable new value
   * @todo: turn into an interface?
   */
  void setScrollable(bool scrollable);

  /**
   * @brief Scroll position within the layout.
   */
  enum class ScrollPosition { Top, Bottom };

  /**
   * Set new scroll position for the next frame.
   * @param position new scroll position
   */
  void setScrollPosition(ScrollPosition position);

  /**
    * Check if horizontal scroll is allowed when content is bigger than the window.
    * @return
    */
  [[nodiscard]] bool isHorizontalScrollEnabled() const;

  /**
    * Enable/disable horizontal scroll when the content is bigger than the window.
    * @param horizontalScroll
    * @todo implement
    */
  void setHorizontalScrollEnabled(bool horizontalScroll);

 protected:
  /**
   * Render collapse button when isCollapsible() == true.
   * @return true if the layout is not collapsed, false otherwise
   * @todo implement
   */
  [[nodiscard]] bool renderCollapseButton();

  /**
   * This needs to be used while rendering inherited layouts.
   * @todo: turn into an interface?
   */
  std::optional<ScrollPosition> nextFrameScrollPosition = std::nullopt;

 private:
  bool drawBorder;
  bool scrollable = false;  // @todo: turn into an interface?
  bool horizontalScrollEnabled = false;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_LAYOUTS_LAYOUT_H
