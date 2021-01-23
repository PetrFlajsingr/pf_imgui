//
// Created by petr on 1/23/21.
//

#include "Hoverable.h"

namespace pf::ui::ig {
void Hoverable::notifyHovered(bool hovered) {
  observableImpl.notify(hovered);
}
}