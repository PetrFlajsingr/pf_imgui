//
// Created by petr on 1/25/21.
//

#include "Positionable.h"

namespace pf::ui::ig {

Positionable::Positionable(const Position &position) : position(position) {}

Position Positionable::getPosition() const { return position; }

void Positionable::setPosition(Position pos) {
  if (pos.x != position.x || pos.y != position.y) {  //-V550
    position = pos;
    notifyPositionChanged(position);
  }
}
void Positionable::updatePosition(Position pos) { position = pos; }

void Positionable::notifyPositionChanged(Position pos) { observableImpl.notify(pos); }

}  // namespace pf::ui::ig
