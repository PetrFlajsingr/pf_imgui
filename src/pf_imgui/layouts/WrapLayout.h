/**
 * @file WrapLayout.h
 * @brief Layout rendering its children in give direction while wrapping each row/column.
 * @author Petr Flajšingr
 * @date 2.9.22
 */

#ifndef PF_IMGUI_WRAPLAYOUT_H
#define PF_IMGUI_WRAPLAYOUT_H

#include "LinearLayout.h"

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
    std::string_view name;                  /*!< Unique name of the element */
    LayoutDirection layoutDirection;        /*!< Direction the element are rendered in */
    Size size;                              /*!< Size of the element */
    ShowBorder showBorder = ShowBorder::No; /*!< Render border around layout's area */
  };
  /**
   * Construct WrapLayout
   * @param config construction args @see WrapLayout::Config
   */
  explicit WrapLayout(Config &&config);

  WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
             ShowBorder showBorder = ShowBorder::No);

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

#endif  //PF_IMGUI_WRAPLAYOUT_H
