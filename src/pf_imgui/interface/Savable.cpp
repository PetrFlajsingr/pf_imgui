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

void Savable::unserialize(const toml::table &src) {
  if (persist) { unserialize_impl(src); }
}

std::optional<toml::table> Savable::serialize() const {
  if (persist) { return serialize_impl(); }
  return std::nullopt;
}
bool Savable::isPersistent() const { return persist; }

void Savable::setPersistent(bool persistent) { persist = persistent; }
}// namespace pf::ui::ig