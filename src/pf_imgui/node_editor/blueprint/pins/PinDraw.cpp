//
// Created by xflajs00 on 29.03.2022.
//

#include "PinDraw.h"
#include <algorithm>
#include <array>

namespace pf::ui::ig::bp {

static const std::array<ImVec2, 3> PinArrowVertices{
    ImVec2{3, 0},
    ImVec2{0, -2},
    ImVec2{0, 2},
};

static const std::array<ImVec2, 4> PinQuadVertices{ImVec2{0, 0}, {6, 0}, {6, 6}, {0, 6}};

static const std::array<ImVec2, 4> PinSmallQuadVertices{ImVec2{0, 0}, {3, 0}, {3, 3}, {0, 3}};

static const std::array<ImVec2, 6> PinExecVertices{ImVec2{0, 0},  ImVec2{7, 0},  ImVec2{10, 6},
                                                   ImVec2{7, 12}, ImVec2{0, 12}, ImVec2{0, 0}};  // -3 -2

void drawPinCircle(ImDrawList *drawList, ImVec2 center, ImColor color, bool filled) {  //-V813
  constexpr auto circleRadius = 4.f;
  if (filled) {
    drawList->AddCircleFilled(center, circleRadius, color);
  } else {
    drawList->AddCircle(center, circleRadius, color);
  }
}

void drawPinArrow(ImDrawList *drawList, ImVec2 position, ImColor color) {  //-V813
  drawList->AddTriangleFilled(position + PinArrowVertices[0], position + PinArrowVertices[1], position + PinArrowVertices[2], color);
}

void drawPinQuad(ImDrawList *drawList, ImVec2 position, ImColor color, bool filled) {  //-V813
  if (filled) {
    drawList->AddQuadFilled(position + PinQuadVertices[0], position + PinQuadVertices[1], position + PinQuadVertices[2],
                            position + PinQuadVertices[3], color);
  } else {
    drawList->AddQuad(position + PinQuadVertices[0], position + PinQuadVertices[1], position + PinQuadVertices[2],
                      position + PinQuadVertices[3], color);
  }
}

void drawPin3by3Quads(ImDrawList *drawList, ImVec2 position, ImColor color, bool filled) {  //-V813
  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      if (x == 1 && y == 1 && !filled) { continue; }
      drawList->AddQuadFilled(position + PinSmallQuadVertices[0] + ImVec2{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f},
                              position + PinSmallQuadVertices[1] + ImVec2{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f},
                              position + PinSmallQuadVertices[2] + ImVec2{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f},
                              position + PinSmallQuadVertices[3] + ImVec2{static_cast<float>(x) * 4.f, static_cast<float>(y) * 4.f}, color);
    }
  }
}

void drawPinExec(ImDrawList *drawList, ImVec2 position, ImColor color, bool filled) {  //-V813
  auto currentVertices = PinExecVertices;
  std::ranges::transform(currentVertices, currentVertices.begin(), [offset = position](ImVec2 vertex) { return vertex + offset; });
  if (filled) {
    drawList->AddConvexPolyFilled(currentVertices.data(), static_cast<int>(currentVertices.size()), color);
  } else {
    currentVertices[0].x -= 1;
    drawList->AddPolyline(currentVertices.data(), static_cast<int>(currentVertices.size()), color, {}, 2.f);
  }
}

}  // namespace pf::ui::ig::bp
