//
// Created by xflajs00 on 11.11.2021.
//

#ifndef PF_IMGUI_PIEMENU_H
#define PF_IMGUI_PIEMENU_H

namespace ImGui {
/* Declaration */
bool BeginPiePopup(const char *pName, int iMouseButton = 0);
void EndPiePopup();

bool PieMenuItem(const char *pName, bool bEnabled = true);
bool BeginPieMenu(const char *pName, bool bEnabled = true);
void EndPieMenu();

}// namespace ImGui

#endif//PF_IMGUI_PIEMENU_H
