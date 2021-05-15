//
// Created by petr on 10/31/20.
//

#include "Resizable.h"

namespace pf::ui::ig {

Resizable::Resizable(const ImVec2 &size) : size(size) {}

Resizable::Resizable(Resizable &&other) noexcept : size(other.size) {}

Resizable &Resizable::operator=(Resizable &&other) noexcept {
  size = other.size;
  return *this;
}

const ImVec2 &Resizable::getSize() const { return size; }

void Resizable::setSize(const ImVec2 &s) {
  if (size.x != s.x || size.y != s.y) {
    size = s;
    notifySizeChanged(size);
  }
}
void Resizable::notifySizeChanged(ImVec2 newSize) { observableImpl.notify(newSize); }

}// namespace pf::ui::ig