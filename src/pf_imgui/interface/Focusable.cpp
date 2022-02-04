//
// Created by petr on 1/23/21.
//

#include "Focusable.h"

namespace pf::ui::ig {

void Focusable::notifyFocusChanged(bool focus) { observableImpl.notify(focus); }

bool Focusable::isFocused() const { return hasFocus(); }

bool Focusable::hasFocus() const { return focused; }

void Focusable::updateFocused(bool focus) {
  if (focus != focused) {
    focused = focus;
    notifyFocusChanged(focused);
  }
}
void Focusable::setFocus() {
  focused = true;
  notifyFocusChanged(focused);
}
}// namespace pf::ui::ig
