//
// Created by petr on 11/2/20.
//

#include "Savable.h"

namespace pf::ui::ig {

Savable::Savable(Persistent persistent) : persist(persistent == Persistent::Yes) {}

Savable::Savable(Savable &&other) noexcept : persist(other.persist) {}

Savable &Savable::operator=(Savable &&other) noexcept {
  persist = other.persist;
  return *this;
}

bool Savable::isPersistent() const { return persist; }

void Savable::setPersistent(bool persistent) { persist = persistent; }

}  // namespace pf::ui::ig
