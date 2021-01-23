//
// Created by petr on 11/14/20.
//

#include "Clickable.h"
#include <algorithm>
#include <ranges>

namespace pf::ui::ig {

void Clickable::notifyOnClick() {
  observableImpl.notify();
}
Subscription Clickable::addClickListener(std::invocable auto fnc) {
  return observableImpl.template addListener(fnc);
}

}// namespace pf::ui::ig