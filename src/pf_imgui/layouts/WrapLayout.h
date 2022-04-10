//
// Created by petr.flajsingr on 2/9/2022.
//

#ifndef PF_IMGUI_WRAPLAYOUT_H
#define PF_IMGUI_WRAPLAYOUT_H

#include "BoxLayout.h"
#include <string>

namespace pf::ui::ig {

// TODO: spacing
class PF_IMGUI_EXPORT WrapLayout : public BoxLayout {
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

  std::vector<float> dimensionPreviousFrame{};
  bool dimensionsCalculated = false;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_WRAPLAYOUT_H
