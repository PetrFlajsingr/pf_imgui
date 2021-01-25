//
// Created by petr on 1/25/21.
//

#include "Anchorable.h"

namespace pf::ui::ig {

Anchorable::Anchorable(Anchor anchor) : anchor(anchor) {}

Anchor Anchorable::getAnchor() const { return anchor; }

void Anchorable::setAnchor(Anchor anch) { anchor = anch; }

}// namespace pf::ui::ig