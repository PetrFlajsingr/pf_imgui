//
// Created by petr on 1/23/21.
//

#include "Focusable.h"

namespace pf::ui::ig {
void Focusable::notifyFocusChanged(bool focused) {
  observableImpl.notify(focused);
}
}
