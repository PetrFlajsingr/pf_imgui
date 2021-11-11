//
// Created by xflajs00 on 11.11.2021.
//

#ifndef PF_IMGUI_PIEMENU_IMPL_H
#define PF_IMGUI_PIEMENU_IMPL_H

namespace ImGuiPie {
bool BeginPiePopup(const char *pName, int iMouseButton = 0);
void EndPiePopup();

bool PieMenuItem(const char *pName, bool bEnabled = true);
bool BeginPieMenu(const char *pName, bool bEnabled = true);
void EndPieMenu();

}// namespace ImGuiPie

#endif//PF_IMGUI_PIEMENU_IMPL_H
