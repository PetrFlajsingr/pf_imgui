//
// Created by xflajs00 on 30.03.2022.
//

#include "Pin.h"

namespace pf::ui::ig::bp {

Pin::Pin(const std::string &name, const std::string &label, Color color) : ig::Pin(name, label), color(color) {}

Color Pin::getColor() const { return color; }

void Pin::setColor(Color newColor) { color = newColor; }

bool Pin::acceptsLinkWith(ig::Pin &other) const {
  if (&getNode() == &other.getNode()) { return false; }
  if (getType() == other.getType()) { return false; }
  return true;
}

toml::table Pin::toToml() const {
  return toml::table{
      {"name", getName()},
      {"label", getLabel()},
      {"color", static_cast<ImU32>(getColor())}
  };
}

void Pin::setFromToml(const toml::table &src) {
  if (auto nameIter = src.find("name"); nameIter != src.end()) {
    if (auto nameToml = nameIter->second.as_string(); nameToml != nullptr) {
      setName(nameToml->get());
    }
  }
  if (auto labelIter = src.find("label"); labelIter != src.end()) {
    if (auto labelToml = labelIter->second.as_string(); labelToml != nullptr) {
      setLabel(labelToml->get());
    }
  }
  if (auto colorIter = src.find("color"); colorIter != src.end()) {
    if (auto colorToml = colorIter->second.as_integer(); colorToml != nullptr) {
      setColor(Color{static_cast<ImU32>(colorToml->get())});
    }
  }
}

void Pin::addLink(Link &link) {
  if (getType() == Pin::Type::Input) {
    std::ranges::for_each(getLinks()
                              | ranges::views::filter([&link](const auto &l) { return l.getId() != link.getId(); }),
                          [](auto &l) { l.invalidate(); });
  }
  ig::Pin::addLink(link);
}

}  // namespace pf::ui::ig::bp