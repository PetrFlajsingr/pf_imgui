//
// Created by petr on 11/14/20.
//

#include "Clickable.h"
#include <algorithm>
#include <ranges>

namespace pf::ui::ig {

void Clickable::notifyOnClick() {
  auto callables = listeners | std::views::values;
  std::ranges::for_each(callables, [&](auto &callable) { callable(); });
}

Clickable::Id Clickable::generateListenerId() { return getNext(idGenerator); }
}// namespace pf::ui::ig