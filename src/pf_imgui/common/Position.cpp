//
// Created by xflajs00 on 02.04.2022.
//

#include "Position.h"

namespace pf::ui::ig {

Position::Position(float initialX, float initialY) : x(initialX), y(initialY) {}

Position::Position(ImVec2 pos) : x(pos.x), y(pos.y) {}

Position Position::LeftTop() { return {0.f, 0.f}; }

Position::operator ImVec2() const { return {x, y}; }

Position Position::moveDelta(float deltaX, float deltaY) const { return {x + deltaX, y + deltaY}; }

}  // namespace pf::ui::ig