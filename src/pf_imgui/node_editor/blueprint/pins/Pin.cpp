//
// Created by xflajs00 on 30.03.2022.
//

#include "Pin.h"
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {

Pin::Pin(const std::string &elementName, const std::string &labelText, Color pinColor)
    : ig::Pin(elementName, labelText), color(pinColor) {}

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
      {"type", getPinTypeId()},
      {"label", label->get()},
      {"pinType", magic_enum::enum_name(getType())},
      {"color", static_cast<ImU32>(getColor())},
      {"validLinkColor", static_cast<ImU32>(getValidLinkPreviewColor())},
      {"invalidLinkColor", static_cast<ImU32>(getInvalidLinkPreviewColor())},
      {"unconnectedLinkColor", static_cast<ImU32>(getUnconnectedLinkPreviewColor())},
      {"validLinkThickness", static_cast<ImU32>(getValidLinkPreviewThickness())},
      {"invalidLinkThickness", static_cast<ImU32>(getInvalidLinkPreviewThickness())},
      {"unconnectedLinkThickness", static_cast<ImU32>(getUnconnectedLinkPreviewThickness())},
  };
}

void Pin::setFromToml(const toml::table &src) {
  if (auto nameIter = src.find("name"); nameIter != src.end()) {
    if (auto nameToml = nameIter->second.as_string(); nameToml != nullptr) { setName(nameToml->get()); }
  }
  if (auto labelIter = src.find("label"); labelIter != src.end()) {
    if (auto labelToml = labelIter->second.as_string(); labelToml != nullptr) { *label.modify() = labelToml->get(); }
  }
  if (auto colorIter = src.find("color"); colorIter != src.end()) {
    if (auto colorToml = colorIter->second.as_integer(); colorToml != nullptr) {
      setColor(Color{static_cast<ImU32>(colorToml->get())});
    }
  }
  if (auto pinTypeIter = src.find("pinType"); pinTypeIter != src.end()) {
    if (auto pinTypeToml = pinTypeIter->second.as_string(); pinTypeToml != nullptr) {
      if (auto pinType = magic_enum::enum_cast<Pin::Type>(pinTypeToml->get()); pinType.has_value()) {
        type = pinType.value();
      }
    }
  }
  if (auto colorIter = src.find("validLinkColor"); colorIter != src.end()) {
    if (auto colorToml = colorIter->second.as_integer(); colorToml != nullptr) {
      setValidLinkPreviewColor(Color{static_cast<ImU32>(colorToml->get())});
    }
  }
  if (auto colorIter = src.find("invalidLinkColor"); colorIter != src.end()) {
    if (auto colorToml = colorIter->second.as_integer(); colorToml != nullptr) {
      setInvalidLinkPreviewColor(Color{static_cast<ImU32>(colorToml->get())});
    }
  }
  if (auto colorIter = src.find("unconnectedLinkColor"); colorIter != src.end()) {
    if (auto colorToml = colorIter->second.as_integer(); colorToml != nullptr) {
      setUnconnectedLinkPreviewColor(Color{static_cast<ImU32>(colorToml->get())});
    }
  }
  if (auto thicknessIter = src.find("validLinkThickness"); thicknessIter != src.end()) {
    if (auto thicknessToml = thicknessIter->second.as_floating_point(); thicknessToml != nullptr) {
      setValidLinkPreviewThickness(static_cast<float>(thicknessToml->get()));
    }
  }
  if (auto thicknessIter = src.find("invalidLinkThickness"); thicknessIter != src.end()) {
    if (auto thicknessToml = thicknessIter->second.as_floating_point(); thicknessToml != nullptr) {
      setInvalidLinkPreviewThickness(static_cast<float>(thicknessToml->get()));
    }
  }
  if (auto thicknessIter = src.find("unconnectedLinkThickness"); thicknessIter != src.end()) {
    if (auto thicknessToml = thicknessIter->second.as_floating_point(); thicknessToml != nullptr) {
      setUnconnectedLinkPreviewThickness(static_cast<float>(thicknessToml->get()));
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