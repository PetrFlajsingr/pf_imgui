//
// Created by xflajs00 on 15.04.2022.
//

#include "Node.h"
#include <pf_imgui/node_editor/blueprint/pins/Pin.h>

namespace pf::ui::ig::bp {

toml::table Node::toToml() const {
  auto result = toml::table{{"name", getName()}};
  auto inputPinsArray = toml::array{};
  std::ranges::for_each(getInputPins(), [&](const ig::Pin &inputPin) {
    if (auto pin = dynamic_cast<const Pin *>(&inputPin); pin != nullptr) { inputPinsArray.push_back(pin->toToml()); }
  });
  auto outputPinsArray = toml::array{};
  std::ranges::for_each(getInputPins(), [&](const ig::Pin &outputPin) {
    if (auto pin = dynamic_cast<const Pin *>(&outputPin); pin != nullptr) { outputPinsArray.push_back(pin->toToml()); }
  });
  result.insert_or_assign("inputs", inputPinsArray);
  result.insert_or_assign("outputs", outputPinsArray);
  return result;
}

void Node::setFromToml(const toml::table &src) {
  if (auto nameIter = src.find("name"); nameIter != src.end()) {
    if (auto nameToml = nameIter->second.as_string(); nameToml != nullptr) {
      setName(nameToml->get());
    }
  }
  // TODO pins
}

}