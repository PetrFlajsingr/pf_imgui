/**
 * @file ResizableLayout.h
 * @brief Base class for layouts which provide resizing.
 * @author Petr Flajšingr
 * @date 24.1.21
 */

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
   * @param showBorder draw layouts border
   */
  ResizableLayout(const std::string &elementName, const Size &size, ShowBorder showBorder);
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_LAYOUTS_RESIZABLELAYOUT_H
