//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_PINDRAW_H
#define IMGUI_EXPERIMENTS_PINDRAW_H

#include "imgui_internal.h"

namespace pf::ui::ig::bp {

void drawPinCircle(ImDrawList *drawList, ImVec2 center, ImColor color, bool filled);

void drawPinArrow(ImDrawList *drawList, ImVec2 position, ImColor color);

void drawPinQuad(ImDrawList *drawList, ImVec2 position, ImColor color, bool filled);

void drawPin3by3Quads(ImDrawList *drawList, ImVec2 position, ImColor color, bool filled);

void drawPinExec(ImDrawList *drawList, ImVec2 position, ImColor color, bool filled);
}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_PINDRAW_H
