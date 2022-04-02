//
// Created by xflajs00 on 17.11.2021.
//

#ifndef PF_IMGUI_DETAIL_GROUPBOX_H
#define PF_IMGUI_DETAIL_GROUPBOX_H

#include <imgui.h>

namespace ImGui {
void BeginGroupPanel(const char *name, const ImVec2 &size);

void EndGroupPanel();

}  // namespace ImGui

#endif  //PF_IMGUI_DETAIL_GROUPBOX_H
