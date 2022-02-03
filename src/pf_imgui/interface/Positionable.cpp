//
// Created by petr on 1/25/21.
//

#include "Positionable.h"

namespace pf::ui::ig {

Position::Position(float x, float y) : x(x), y(y) {}

Position::Position(ImVec2 pos) : x(pos.x), y(pos.y) {}

Position Position::LeftTop() { return {0.f, 0.f}; }

ImVec2 Position::asImVec() const { return {x, y}; }

Position Position::moveDelta(float deltaX, float deltaY) const { return {x + deltaX, y + deltaY}; }

Positionable::Positionable(const Position &position) : position(position) {}

Position Positionable::getPosition() const { return position; }

void Positionable::setPosition(Position pos) {
  if (pos.x != position.x || pos.y != position.y) {
    position = pos;
    notifyPositionChanged(position);
  }
}
void Positionable::updatePosition(Position pos) { position = pos; }

void Positionable::notifyPositionChanged(Position pos) { observableImpl.notify(pos); }

}// namespace pf::ui::ig