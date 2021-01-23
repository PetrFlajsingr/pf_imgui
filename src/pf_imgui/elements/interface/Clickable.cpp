//
// Created by petr on 11/14/20.
//

#include "Clickable.h"
#include <algorithm>
#include <ranges>

namespace pf::ui::ig {

void Clickable::notifyOnClick() { observableImpl.notify(); }

}// namespace pf::ui::ig