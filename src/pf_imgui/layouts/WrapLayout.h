//
// Created by petr.flajsingr on 2/9/2022.
//

#ifndef PF_IMGUI_WRAPLAYOUT_H
#define PF_IMGUI_WRAPLAYOUT_H

#include "BoxLayout.h"
#include <string>

namespace pf::ui::ig {

class WrapLayout : public BoxLayout {
 public:
  /**
   * @brief Struct for construction of WrapLayout.
   */
  struct Config {
    using Parent = WrapLayout;
    std::string_view name;                           /*!< Unique name of the element */
    LayoutDirection layoutDirection;                 /*!< Direction the element are rendered in */
    Size size;                                       /*!< Size of the element */
    AllowCollapse allowCollapse = AllowCollapse::No; /*!< Allow collapse functionality */
    ShowBorder showBorder = ShowBorder::No;          /*!< Render border around layout's area */
    Persistent persistent = Persistent::No;          /*!< Allow state saving to disk */
  };
  /**
   * Construct WrapLayout
   * @param config construction args @see WrapLayout::Config
   */
  explicit WrapLayout(Config &&config);

  WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
             AllowCollapse allowCollapse = AllowCollapse::No, ShowBorder showBorder = ShowBorder::No,
             Persistent persistent = Persistent::No);

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
//
// Created by petr.flajsingr on 2/9/2022.
//

#ifndef PF_IMGUI_WRAPLAYOUT_H
#define PF_IMGUI_WRAPLAYOUT_H

#include "BoxLayout.h"
#include <string>

namespace pf::ui::ig {

class WrapLayout : public BoxLayout {
 public:
  /**
   * @brief Struct for construction of WrapLayout.
   */
  struct Config {
    using Parent = WrapLayout;
    std::string_view name;                           /*!< Unique name of the element */
    LayoutDirection layoutDirection;                 /*!< Direction the element are rendered in */
    Size size;                                       /*!< Size of the element */
    AllowCollapse allowCollapse = AllowCollapse::No; /*!< Allow collapse functionality */
    ShowBorder showBorder = ShowBorder::No;          /*!< Render border around layout's area */
    Persistent persistent = Persistent::No;          /*!< Allow state saving to disk */
  };
  /**
   * Construct WrapLayout
   * @param config construction args @see WrapLayout::Config
   */
  explicit WrapLayout(Config &&config);

  WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
             AllowCollapse allowCollapse = AllowCollapse::No, ShowBorder showBorder = ShowBorder::No,
             Persistent persistent = Persistent::No);

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
