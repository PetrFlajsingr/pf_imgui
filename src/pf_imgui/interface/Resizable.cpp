//
// Created by petr on 10/31/20.
//

#include "Resizable.h"

namespace pf::ui::ig {

Size::Size(const Width &width, const Height &height) : width(width), height(height) {}

bool Size::operator==(const Size &rhs) const { return width == rhs.width && height == rhs.height; }

bool Size::operator!=(const Size &rhs) const { return !(rhs == *this); }

Size Size::Auto() { return {Width::Auto(), Height::Auto()}; }

Size Size::Fill() { return {Width::Fill(), Height::Fill()}; }

ImVec2 Size::asImVec() const { return ImVec2{width.value, height.value}; }

Size Size::FillWidth() { return {Width::Fill(), Height::Auto()}; }

Size::Size(ImVec2 vec) : width(vec.x), height(vec.y) {}

Size Size::FillHeight() { return Size(Width::Auto(), Height::Fill()); }

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
void Resizable::notifySizeChanged(Size newSize) { observableImpl.notify(newSize); }

}// namespace pf::ui::ig