//
// Created by petr on 10/31/20.
//

#include "Resizable.h"
#include <utility>

namespace pf::ui::ig {

Resizable::Resizable(const ImVec2 &size) : size(size) {}

Resizable::Resizable(Resizable &&other) noexcept : size(other.size) {}

Resizable &Resizable::operator=(Resizable &&other) noexcept {
  size = other.size;
  return *this;
}

const ImVec2 &Resizable::getSize() const { return size; }

void Resizable::setSize(const ImVec2 &s) { size = s; }

}// namespace pf::ui::ig