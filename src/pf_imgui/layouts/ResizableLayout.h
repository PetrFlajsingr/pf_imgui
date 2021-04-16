//
// Created by petr on 1/24/21.
//

#ifndef PF_IMGUI_LAYOUTS_RESIZABLELAYOUT_H
#define PF_IMGUI_LAYOUTS_RESIZABLELAYOUT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Layout.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief An interface for Layouts, which support changing their size.
 *
 * Interface for common capabilities required by layouts.
 */
class PF_IMGUI_EXPORT ResizableLayout : public Layout, public Resizable {
 public:
  /**
   * Construct ResizableLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param allowCollapse enable collapse button
   * @param showBorder draw layouts border
   * @param persistent allow state saving
   */
  ResizableLayout(const std::string &elementName, const ImVec2 &size, AllowCollapse allowCollapse,
                  ShowBorder showBorder, Persistent persistent);

 protected:
  /**
   * Check whether the layout is collapsed or not and calculate its size based on that.
   * @attention Should be used in descendants to provide common functionality.
   * @return size of the layout for rendering purposes
   */
  [[nodiscard]] ImVec2 getSizeIfCollapsed() const;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_LAYOUTS_RESIZABLELAYOUT_H
