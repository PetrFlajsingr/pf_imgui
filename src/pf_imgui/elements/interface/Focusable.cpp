//
// Created by petr on 1/23/21.
//

#include "Focusable.h"

namespace pf::ui::ig {

void Focusable::notifyFocusChanged(bool focus) {
  observableImpl.notify(focus);
}

bool Focusable::isFocused() const {
  return focused;
}
void Focusable::setFocusedWithoutDemandingFocusChange(bool newFocused) {
  if (newFocused != focused) {
    focused = newFocused;
    notifyFocusChanged(focused);
  }
}

}
