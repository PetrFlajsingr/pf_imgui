//
// Created by petr on 1/24/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H

#include "Layout.h"
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Resizable.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT BoxLayout : public Layout, public Resizable {
 public:
  BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const ImVec2 &size = ImVec2{0, 0},
            bool showBorder = false);

  [[nodiscard]] LayoutDirection getLayoutDirection() const;
  void setLayoutDirection(LayoutDirection layoutDirection);

  [[nodiscard]] bool isShowBorder() const;
  void setShowBorder(bool showBorder);

 protected:
  void renderImpl() override;

 private:
  LayoutDirection layoutDirection;
  bool showBorder;

  void renderTopToBottom();
  void renderLeftToRight();
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H
