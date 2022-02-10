//
// Created by petr.flajsingr on 2/9/2022.
//

#ifndef PF_IMGUI_WRAPLAYOUT_H
#define PF_IMGUI_WRAPLAYOUT_H

#include "BoxLayout.h"

namespace pf::ui::ig {

class WrapLayout : public BoxLayout {
 public:
  WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
             AllowCollapse allowCollapse = AllowCollapse::No, ShowBorder showBorder = ShowBorder::No,
             Persistent persistent = Persistent::No);

  WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size, ShowBorder showBorder,
             Persistent persistent = Persistent::No);

  WrapLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
             AllowCollapse allowCollapse, Persistent persistent = Persistent::No);

 protected:
  void renderImpl() override;

 private:
  void renderLeftToRight();
  void renderTopToBottom();
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_WRAPLAYOUT_H