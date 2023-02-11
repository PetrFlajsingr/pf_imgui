//
// Created by xflajs00 on 17.04.2022.
//

#include "CustomIconButtonImpls.h"
#include <imgui_internal.h>
#include <pf_common/array.h>

namespace pf::ui::ig {

void TrashcanButton::renderIcon(ImDrawList *drawList, ImVec2 cursorPos) {
  drawList->AddRectFilled(cursorPos + ImVec2{8, 9}, cursorPos + ImVec2{15, 18}, static_cast<ImU32>(getIconColor()));
  drawList->AddRectFilled(cursorPos + ImVec2{7, 6}, cursorPos + ImVec2{16, 8}, static_cast<ImU32>(getIconColor()));
  drawList->AddRectFilled(cursorPos + ImVec2{10, 5}, cursorPos + ImVec2{13, 6}, static_cast<ImU32>(getIconColor()));
}

void WrapTextToggle::renderIcon(ImDrawList *drawList, ImVec2 cursorPos) {
  drawList->AddRectFilled(cursorPos + ImVec2{6, 6}, cursorPos + ImVec2{18, 8}, static_cast<ImU32>(getIconColor()));
  drawList->AddRectFilled(cursorPos + ImVec2{6, 11}, cursorPos + ImVec2{16, 13}, static_cast<ImU32>(getIconColor()));
  drawList->AddRectFilled(cursorPos + ImVec2{6, 16}, cursorPos + ImVec2{9, 18}, static_cast<ImU32>(getIconColor()));

  const auto points = make_array<ImVec2>(cursorPos + ImVec2{16, 12}, cursorPos + ImVec2{18, 14}, cursorPos + ImVec2{18, 15},
                                         cursorPos + ImVec2{16, 17}, cursorPos + ImVec2{14, 17});
  drawList->AddPolyline(points.data(), static_cast<int>(points.size()), static_cast<ImU32>(getIconColor()), {}, 2.f);
  drawList->AddTriangleFilled(cursorPos + ImVec2{14, 19}, cursorPos + ImVec2{14, 15}, cursorPos + ImVec2{12, 17},
                              static_cast<ImU32>(getIconColor()));
}

void ScrollToEndToggle::renderIcon(ImDrawList *drawList, ImVec2 cursorPos) {
  drawList->AddRectFilled(cursorPos + ImVec2{6, 6}, cursorPos + ImVec2{9, 8}, static_cast<ImU32>(getIconColor()));
  drawList->AddRectFilled(cursorPos + ImVec2{6, 11}, cursorPos + ImVec2{9, 13}, static_cast<ImU32>(getIconColor()));
  drawList->AddRectFilled(cursorPos + ImVec2{6, 16}, cursorPos + ImVec2{18, 18}, static_cast<ImU32>(getIconColor()));
  drawList->AddRectFilled(cursorPos + ImVec2{12, 5}, cursorPos + ImVec2{15, 9}, static_cast<ImU32>(getIconColor()));
  drawList->AddTriangleFilled(cursorPos + ImVec2{9, 9}, cursorPos + ImVec2{18, 9}, cursorPos + ImVec2{14, 14},
                              static_cast<ImU32>(getIconColor()));
}

void CopyToClipboardButton::renderIcon(ImDrawList *drawList, ImVec2 cursorPos) {
  drawList->PushClipRect(cursorPos + ImVec2{6, 6}, cursorPos + ImVec2{13, 9});
  drawList->AddRect(cursorPos + ImVec2{6, 6}, cursorPos + ImVec2{13, 15}, static_cast<ImU32>(getIconColor()));
  drawList->PopClipRect();

  drawList->PushClipRect(cursorPos + ImVec2{6, 6}, cursorPos + ImVec2{9, 15});
  drawList->AddRect(cursorPos + ImVec2{6, 6}, cursorPos + ImVec2{13, 15}, static_cast<ImU32>(getIconColor()));
  drawList->PopClipRect();

  drawList->AddRect(cursorPos + ImVec2{9, 9}, cursorPos + ImVec2{16, 18}, static_cast<ImU32>(getIconColor()));
}

}  // namespace pf::ui::ig