//
// Created by petr on 1/25/21.
//

#include "Positionable.h"

namespace pf::ui::ig {

Positionable::Positionable(const ImVec2 &position) : position(position) {}

ImVec2 Positionable::getPosition() const { return position; }

void Positionable::setPosition(ImVec2 pos) { position = pos; }

}// namespace pf::ui::ig