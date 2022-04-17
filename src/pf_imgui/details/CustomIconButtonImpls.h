//
// Created by xflajs00 on 17.04.2022.
//

#ifndef PF_IMGUI_DETAILS_CUSTOMICONBUTTONIMPLS_H
#define PF_IMGUI_DETAILS_CUSTOMICONBUTTONIMPLS_H

#include <pf_imgui/elements/CustomIconButtonBehavior.h>

namespace pf::ui::ig {

class TrashcanButton : public CustomIconButton {
 public:
  using CustomIconButton::CustomIconButton;

 protected:
  void renderIcon(ImDrawList *drawList, ImVec2 cursorPos) override;
};

class WrapTextToggle : public CustomIconToggle {
 public:
  using CustomIconToggle::CustomIconToggle;

 protected:
  void renderIcon(ImDrawList *drawList, ImVec2 cursorPos) override;
};

class ScrollToEndToggle : public CustomIconToggle {
 public:
  using CustomIconToggle::CustomIconToggle;

 protected:
  void renderIcon(ImDrawList *drawList, ImVec2 cursorPos) override;
};

class CopyToClipboardButton : public CustomIconButton {
 public:
  using CustomIconButton::CustomIconButton;

 protected:
  void renderIcon(ImDrawList *drawList, ImVec2 cursorPos) override;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_DETAILS_CUSTOMICONBUTTONIMPLS_H
