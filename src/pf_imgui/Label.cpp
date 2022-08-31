//
// Created by xflajs00 on 22.08.2022.
//

#include "Label.h"

namespace pf::ui::ig {

Label::Label(std::string value) : val(std::move(value)) {}

Label &Label::operator=(const std::string &other) {
  val = other;
  return *this;
}

Label &Label::operator=(std::string &&other) {
  val = std::move(other);
  return *this;
}

const std::string &Label::get() const {
  if (visibility == Visibility::Visible) { return val; }
  return EMPTY_LABEL;
}

void Label::setVisibility(Visibility vis) { visibility = vis; }

void Label::show() { visibility = Visibility::Visible; }

void Label::hide() { visibility = Visibility::Invisible; }

Visibility Label::getVisibility() const { return visibility; }

}  // namespace pf::ui::ig