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

bool Pin::acceptsNewLinks() const { return true; }

bool Pin::acceptsLinkWith([[maybe_unused]] Pin &other) const { return true; }

PopupMenu &Pin::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>(getName() + "_popup"); }
  return *popupMenu;
}

bool Pin::hasPopupMenu() const { return popupMenu != nullptr; }

void Pin::removePopupMenu() { popupMenu = nullptr; }

void Pin::renderImpl() {
  ImGui::BeginHorizontal(getId().AsPointer());
  {
    ax::NodeEditor::BeginPin(getId(), static_cast<ax::NodeEditor::PinKind>(getType()));
    auto endPin = RAII{ax::NodeEditor::EndPin};

    if (getType() == Type::Input) {
      ax::NodeEditor::PinPivotAlignment(ImVec2(0.f, 0.5f));
      ax::NodeEditor::PinPivotSize(ImVec2(0, 0));
      renderIcon();
      ImGui::Spring(0);
      ImGui::BeginVertical((getName() + "_info").c_str());
      renderInfo();
      ImGui::EndVertical();
    } else {
      ax::NodeEditor::PinPivotAlignment(ImVec2(1.0f, 0.5f));
      ax::NodeEditor::PinPivotSize(ImVec2(0, 0));

      ImGui::Spring(1);
      ImGui::BeginVertical((getName() + "_info").c_str());
      renderInfo();
      ImGui::EndVertical();
      ImGui::Spring(0);
      renderIcon();
    }
  }
  ImGui::EndHorizontal();

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

void Pin::renderInfo() { ImGui::Text(getLabel().c_str()); }

const ImVec4 &Pin::getValidLinkPreviewColor() const { return validLinkPreviewColor; }

void Pin::setValidLinkPreviewColor(const ImVec4 &color) { validLinkPreviewColor = color; }

float Pin::getValidLinkPreviewThickness() const { return validLinkPreviewThickness; }

void Pin::setValidLinkPreviewThickness(float thickness) { validLinkPreviewThickness = thickness; }

const ImVec4 &Pin::getInvalidLinkPreviewColor() const { return invalidLinkPreviewColor; }

void Pin::setInvalidLinkPreviewColor(const ImVec4 &color) { invalidLinkPreviewColor = color; }

float Pin::getInvalidLinkPreviewThickness() const { return invalidLinkPreviewThickness; }

void Pin::setInvalidLinkPreviewThickness(float thickness) { invalidLinkPreviewThickness = thickness; }

const ImVec4 &Pin::getUnconnectedLinkPreviewColor() const { return unconnectedLinkPreviewColor; }

void Pin::setUnconnectedLinkPreviewColor(const ImVec4 &color) { unconnectedLinkPreviewColor = color; }

float Pin::getUnconnectedLinkPreviewThickness() const { return unconnectedLinkPreviewThickness; }

void Pin::setUnconnectedLinkPreviewThickness(float thickness) { unconnectedLinkPreviewThickness = thickness; }

}  // namespace pf::ui::ig