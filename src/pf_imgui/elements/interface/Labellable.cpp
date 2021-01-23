//
// Created by petr on 10/31/20.
//

#include "Labellable.h"
#include <string>
#include <utility>

namespace pf::ui::ig {
Labellable::Labellable(std::string label_) : label_(std::move(label_)) {}

Labellable::Labellable(Labellable &&other) noexcept : label_(std::move(other.label_)) {}

Labellable &Labellable::operator=(Labellable &&other) noexcept {
  label_ = std::move(other.label_);
  return *this;
}

const std::string &Labellable::getLabel() const { return label_; }

void Labellable::setLabel(const std::string &cap) { label_ = cap; }

}// namespace pf::ui::ig