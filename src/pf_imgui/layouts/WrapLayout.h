/**
 * @file WrapLayout.h
 * @brief Layout rendering its children in give direction while wrapping each row/column.
 * @author Petr Flajšingr
 * @date 2.9.22
 */

#ifndef PF_IMGUI_LAYOUTS_WRAPLAYOUT_H
#define PF_IMGUI_LAYOUTS_WRAPLAYOUT_H

#include "LinearLayout.h"
#include <pf_common/Explicit.h>

namespace pf::ui::ig {
/**
 * @brief Layout rendering its children in give direction while wrapping each row/column.
 */
class PF_IMGUI_EXPORT WrapLayout : public LinearLayout {
 public:
  /**
   * @brief Struct for construction of WrapLayout.
   */
  struct Config {
    using Parent = WrapLayout;
    Explicit<std::string_view> name;           /*!< Unique name of the element */
    Explicit<LayoutDirection> layoutDirection; /*!< Direction the element are rendered in */
    Explicit<Size> size;                       /*!< Size of the element */
    bool showBorder = false;                   /*!< Render border around layout's area */
  };
  /**
   * Construct WrapLayout
   * @param config construction args @see WrapLayout::Config
   */
  explicit WrapLayout(Config &&config);
  /**
   * Construct WrapLayout
   * @param elementName unique name of the element
   * @param layoutDirection direction of the layout flow
   * @param initialSize size of the layout
   * @param showBorder enable/disable border draw
   */
  WrapLayout(std::string_view elementName, LayoutDirection layoutDirection, Size initialSize, ShowBorder showBorder = ShowBorder::No);

 protected:
  void renderImpl() override;

 private:
  void renderLeftToRight();
  void renderTopToBottom();

  LayoutDirection direction;
  std::vector<float> dimensionPreviousFrame{};
  bool dimensionsCalculated = false;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_LAYOUTS_WRAPLAYOUT_H
