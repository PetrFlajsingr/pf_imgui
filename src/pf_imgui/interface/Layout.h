/**
 * @file Layout.h
 * @brief Base class for layouts.
 * @author Petr Flajšingr
 * @date 25.1.21
 */

#ifndef PF_IMGUI_LAYOUTS_LAYOUT_H
#define PF_IMGUI_LAYOUTS_LAYOUT_H

#include <pf_imgui/_export.h>
#include <src/pf_imgui/interface/Collapsible.h>
#include <src/pf_imgui/interface/Element.h>
#include <src/pf_imgui/interface/Resizable.h>
#include <vector>

namespace pf::ui::ig {

// TODO: collapse support in children
// TODO: flags getter for children
/**
 * @brief Base class for layouts.
 *
 * Provides basic functionality for layouts - border drawing and scrolling.
 */
class PF_IMGUI_EXPORT Layout : public Element, public Collapsible {
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
   */
  [[nodiscard]] bool isScrollable() const;
  /**
   * Set layout scrollable
   * @param scrollable new value
   */
  void setScrollable(bool scrollable);

  /**
   * Get all renderables stored inside the layout.
   * @return vector of pointers to renderables
   */
  [[nodiscard]] virtual std::vector<Renderable *> getRenderables() = 0;

  /**
   * @brief Scroll position within the layout.
   */
  enum class ScrollPosition { Top, Bottom };

  /**
   * Set new scroll position for the next frame.
   * @param position new scroll position
   */
  void setScrollPosition(ScrollPosition position);

 protected:
  /**
   * Render collapse button when isCollapsible() == true.
   * @return true if the layout is not collapsed, false otherwise
   */
  [[nodiscard]] bool renderCollapseButton();

  /**
   * This needs to be used while rendering inherited layouts.
   * @todo: Implement in all layouts.
   */
  std::optional<ScrollPosition> nextFrameScrollPosition = std::nullopt;

 private:
  bool drawBorder;
  bool scrollable = false;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_LAYOUTS_LAYOUT_H
