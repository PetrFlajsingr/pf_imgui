//
// Created by petr on 10/31/20.
//

#include "Labellable.h"
#include <string>
#include <utility>

namespace pf::ui::ig {
Labellable::Labellable(std::unique_ptr<Resource<std::string>> label_) : label_(std::move(label_)) {}

Labellable::Labellable(Labellable &&other) noexcept : label_(std::move(other.label_)) {}

Labellable &Labellable::operator=(Labellable &&other) noexcept {
  label_ = std::move(other.label_);
  return *this;
}

const Resource<std::string> &Labellable::getLabel() const {
  return *label_;
}

void Labellable::setLabel(std::unique_ptr<Resource<std::string>> newLabel) {
  label_ = std::move(newLabel);
}

void Labellable::setLabelVisible(Visibility newVisibility) { visibility = newVisibility; }

void Labellable::showLabel() { setLabelVisible(Visibility::Visible); }

void Labellable::hideLabel() { setLabelVisible(Visibility::Invisible); }

bool Labellable::isLabelVisible() const { return visibility == Visibility::Visible; }

}  // namespace pf::ui::ig
