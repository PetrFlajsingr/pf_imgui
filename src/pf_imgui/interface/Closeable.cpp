//
// Created by xflajs00 on 16.11.2021.
//

#include "Closeable.h"

namespace pf::ui::ig {

bool Closeable::isCloseable() const { return closeable; }

void Closeable::setCloseable(bool newCloseable) { closeable = newCloseable; }

void Closeable::notifyClosed() { closeObservableImpl.notify(); }
}// namespace pf::ui::ig
