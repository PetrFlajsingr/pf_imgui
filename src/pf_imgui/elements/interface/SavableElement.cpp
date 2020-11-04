//
// Created by petr on 11/2/20.
//

#include "SavableElement.h"

namespace pf::ui::ig {
SavableElement::SavableElement(const std::string &elementName, Persistent persistent)
    : Element(elementName), persist(persistent == Persistent::Yes) {}
void SavableElement::unserialize(const toml::table &src) {
  if (persist) {
    unserialize_impl(src);
  }
}
std::optional<toml::table> SavableElement::serialize() {
  if (persist) {
    return serialize_impl();
  }
  return std::nullopt;
}
}// namespace pf::ui::ig