//
// Created by petr on 1/23/21.
//

#include "Hoverable.h"

namespace pf::ui::ig {
void Hoverable::notifyHovered(bool hover) { observableImpl.notify(hover); }

bool Hoverable::isHovered() const { return hovered; }

void Hoverable::setHovered(bool hover) {
  if (hover != hovered) {
    hovered = hover;
    notifyHovered(hovered);
  }
}

}  // namespace pf::ui::ig
