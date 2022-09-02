//
// Created by xflajs00 on 15.04.2022.
//

#include "Node.h"
#include <pf_imgui/node_editor/blueprint/common.h>
#include <pf_imgui/node_editor/blueprint/pins/Pin.h>

namespace pf::ui::ig::bp {

toml::table Node::toToml() const {
  auto result = toml::table{{"name", getName()},
                            {"type", getNodeTypeId()},
                            {"positionX", position->x},
                            {"positionY", position->y}};
  auto inputPinsArray = toml::array{};
  std::ranges::for_each(getInputPins(), [&](const ig::Pin &inputPin) {
    if (auto pin = dynamic_cast<const Pin *>(&inputPin); pin != nullptr) { inputPinsArray.push_back(pin->toToml()); }
  });
  auto outputPinsArray = toml::array{};
  std::ranges::for_each(getOutputPins(), [&](const ig::Pin &outputPin) {
    if (auto pin = dynamic_cast<const Pin *>(&outputPin); pin != nullptr) { outputPinsArray.push_back(pin->toToml()); }
  });
  result.insert_or_assign("inputs", inputPinsArray);
  result.insert_or_assign("outputs", outputPinsArray);
  return result;
}

void Node::setFromToml(const toml::table &src) {
  if (auto nameIter = src.find("name"); nameIter != src.end()) {
    if (auto nameToml = nameIter->second.as_string(); nameToml != nullptr) { setName(nameToml->get()); }
  }
  if (auto positionXIter = src.find("positionX"); positionXIter != src.end()) {
    if (auto positionXToml = positionXIter->second.as_floating_point(); positionXToml != nullptr) {
      if (auto positionYIter = src.find("positionY"); positionYIter != src.end()) {
        if (auto positionYToml = positionYIter->second.as_floating_point(); positionYToml != nullptr) {
          *position.modify() =
              Position{static_cast<float>(positionXToml->get()), static_cast<float>(positionYToml->get())};
        }
      }
    }
  }
  const auto pinFromToml = [&](const toml::table &pinSrc) -> std::optional<std::unique_ptr<Pin>> {
    if (auto typeIter = pinSrc.find("type"); typeIter != pinSrc.end()) {
      if (auto typeValue = typeIter->second.as_string(); typeValue != nullptr) {
        return NodeEditorLoading::Get()->constructPin(typeValue->get(), this, pinSrc);
      }
    }
    return std::nullopt;
  };
  if (auto inputsIter = src.find("inputs"); inputsIter != src.end()) {
    if (auto inputsToml = inputsIter->second.as_array(); inputsToml != nullptr) {
      std::ranges::for_each(*inputsToml, [&](const auto &item) {
        if (auto pinToml = item.as_table(); pinToml != nullptr) {
          if (auto newPin = pinFromToml(*pinToml); newPin.has_value()) {
            (*newPin)->id = getNextPinId();
            inputPins.emplace_back(std::move(*newPin));
          }
        }
      });
    }
  }
  if (auto outputsIter = src.find("outputs"); outputsIter != src.end()) {
    if (auto outputsToml = outputsIter->second.as_array(); outputsToml != nullptr) {
      std::ranges::for_each(*outputsToml, [&](const auto &item) {
        if (auto pinToml = item.as_table(); pinToml != nullptr) {
          if (auto newPin = pinFromToml(*pinToml); newPin.has_value()) {
            (*newPin)->id = getNextPinId();
            outputPins.emplace_back(std::move(*newPin));
          }
        }
      });
    }
  }
}

}  // namespace pf::ui::ig::bp