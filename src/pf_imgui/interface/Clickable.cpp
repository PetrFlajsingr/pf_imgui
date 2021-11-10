//
// Created by petr on 11/14/20.
//

#include "Clickable.h"

namespace pf::ui::ig {

void Clickable::notifyOnClick() { observableImpl.notify(); }

}// namespace pf::ui::ig