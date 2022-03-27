//
// Created by xflajs00 on 27.03.2022.
//

#include "Pin.h"
#include "Link.h"
#include "Node.h"
#include "pf_common/RAII.h"
#include <algorithm>

namespace pf::ui::ig {

Pin::Pin(const std::string &name, const std::string &label) : Renderable(name), Labellable(label) {}

Pin::~Pin() {
  std::ranges::for_each(links, [](auto &link) { link->invalidate(); });
}

ax::NodeEditor::PinId Pin::getId() const { return id; }

Pin::Type Pin::getType() const { return type; }

const std::vector<std::shared_ptr<Link>> &Pin::getLinks() const { return links; }

Node &Pin::getNode() { return *parent; }

const Node &Pin::getNode() const { return *parent; }

bool Pin::hasAnyValidLinks() const {
  return std::ranges::any_of(links, [](const auto &link) { return link->isValid(); });
}

PopupMenu &Pin::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>(getName() + "_popup"); }
  return *popupMenu;
}

bool Pin::hasPopupMenu() const { return popupMenu != nullptr; }

void Pin::removePopupMenu() { popupMenu = nullptr; }

void Pin::renderImpl() {
  ax::NodeEditor::BeginPin(id, static_cast<ax::NodeEditor::PinKind>(type));
  auto endPin = RAII{ax::NodeEditor::EndPin};

  if (type == Type::Input) {
    renderIcon();
    ImGui::SameLine();
    ImGui::Text(getLabel().c_str());
  } else {
    ImGui::Text(getLabel().c_str());
    ImGui::SameLine();
    renderIcon();
  }

  if (--sinceLastLinkCleanup == 0) {
    sinceLastLinkCleanup = LINK_CLEANUP_FREQUENCY;
    auto [beginRm, endRm] = std::ranges::remove_if(links, [](const auto &link) { return !link->isValid(); });
    links.erase(beginRm, endRm);
  }
}

void Pin::renderIcon() {
  std::string prefix;
  std::string postfix;
  if (type == Type::Input) {
    prefix = "->";
  } else {
    postfix = "->";
  }
  std::string infix;
  if (hasAnyValidLinks()) {
    infix = "(X)";
  } else {
    infix = "( )";
  }
  ImGui::Text((prefix + infix + postfix).c_str());
}
const ImVec4 &Pin::getValidLinkPreviewColor() const { return validLinkPreviewColor; }

void Pin::setValidLinkPreviewColor(const ImVec4 &validLinkPreviewColor) {
  Pin::validLinkPreviewColor = validLinkPreviewColor;
}

float Pin::getValidLinkPreviewThickness() const { return validLinkPreviewThickness; }

void Pin::setValidLinkPreviewThickness(float validLinkPreviewThickness) {
  Pin::validLinkPreviewThickness = validLinkPreviewThickness;
}

const ImVec4 &Pin::getInvalidLinkPreviewColor() const { return invalidLinkPreviewColor; }

void Pin::setInvalidLinkPreviewColor(const ImVec4 &invalidLinkPreviewColor) {
  Pin::invalidLinkPreviewColor = invalidLinkPreviewColor;
}

float Pin::getInvalidLinkPreviewThickness() const { return invalidLinkPreviewThickness; }

void Pin::setInvalidLinkPreviewThickness(float invalidLinkPreviewThickness) {
  Pin::invalidLinkPreviewThickness = invalidLinkPreviewThickness;
}

const ImVec4 &Pin::getUnconnectedLinkPreviewColor() const { return unconnectedLinkPreviewColor; }

void Pin::setUnconnectedLinkPreviewColor(const ImVec4 &unconnectedLinkPreviewColor) {
  Pin::unconnectedLinkPreviewColor = unconnectedLinkPreviewColor;
}

float Pin::getUnconnectedLinkPreviewThickness() const { return unconnectedLinkPreviewThickness; }

void Pin::setUnconnectedLinkPreviewThickness(float unconnectedLinkPreviewThickness) {
  Pin::unconnectedLinkPreviewThickness = unconnectedLinkPreviewThickness;
}

}  // namespace pf::ui::ig