//
// Created by petr on 10/31/20.
//

#include "Resizable.h"

namespace pf::ui::ig {

Resizable::Resizable(const Size &s) : size(s) {}

Resizable::Resizable(Resizable &&other) noexcept : size(other.size) {}

Resizable &Resizable::operator=(Resizable &&other) noexcept {
  size = other.size;
  return *this;
}
const Size &Resizable::getSize() const { return size; }

void Resizable::setSize(const Size &s) {
  if (size != s) {
    size = s;
    notifySizeChanged(size);
  }
}

void Resizable::setSizeInner(Size newSize) { size = newSize; }

void Resizable::notifySizeChanged(Size newSize) { observableImpl.notify(newSize); }

}  // namespace pf::ui::ig
