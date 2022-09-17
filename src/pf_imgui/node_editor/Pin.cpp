//
// Created by xflajs00 on 27.03.2022.
//

#include "Pin.h"
#include "Link.h"
#include "Node.h"
#include "NodeEditor.h"
#include <algorithm>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Pin::Pin(Pin::Config &&config) : Renderable(config.name.value), label(std::string{config.label.value}) {}

Pin::Pin(std::string_view elementName, std::string_view labelText)
    : Renderable(elementName), label(std::string{labelText}) {}

ax::NodeEditor::PinId Pin::getId() const { return id; }

Pin::Type Pin::getType() const { return type; }

Node &Pin::getNode() { return *parent; }

const Node &Pin::getNode() const { return *parent; }

bool Pin::hasAnyValidLinks() const {
  return std::ranges::any_of(getLinks(), [](const auto &link) { return link.isValid(); });
}

void Pin::clearLinks() {
  std::ranges::for_each(getLinks(), [](auto &link) { link.invalidate(); });
  getNode().getNodeEditor().markLinksDirty();
}

bool Pin::acceptsNewLinks() const { return true; }

bool Pin::acceptsLinkWith([[maybe_unused]] Pin &other) const { return true; }

PopupMenu &Pin::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>("popup"); }
  return *popupMenu;
}

bool Pin::hasPopupMenu() const { return popupMenu != nullptr; }

void Pin::removePopupMenu() { popupMenu = nullptr; }

void Pin::renderImpl() {
  ImGui::BeginHorizontal(getId().AsPointer());
  {
    ax::NodeEditor::BeginPin(getId(), static_cast<ax::NodeEditor::PinKind>(getType()));
    [[maybe_unused]] auto endPin = RAII{ax::NodeEditor::EndPin};

    if (getType() == Type::Input) {
      ax::NodeEditor::PinPivotAlignment(ImVec2(0.f, 0.5f));
      ax::NodeEditor::PinPivotSize(ImVec2(0, 0));
      renderIcon();
      ImGui::Spring(0);
      ImGui::BeginVertical("info");
      renderInfo();
      ImGui::EndVertical();
    } else {
      ax::NodeEditor::PinPivotAlignment(ImVec2(1.0f, 0.5f));
      ax::NodeEditor::PinPivotSize(ImVec2(0, 0));

      ImGui::Spring(1);
      ImGui::BeginVertical("info");
      renderInfo();
      ImGui::EndVertical();
      ImGui::Spring(0);
      renderIcon();
    }
  }
  ImGui::EndHorizontal();
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

void Pin::renderInfo() { ImGui::Text(label->get().c_str()); }

void Pin::addLink(Link &link) { Event_notify(linkChangedEvent, link, true); }

Color Pin::getValidLinkPreviewColor() const { return validLinkPreviewColor; }

void Pin::setValidLinkPreviewColor(Color color) { validLinkPreviewColor = color; }

float Pin::getValidLinkPreviewThickness() const { return validLinkPreviewThickness; }

void Pin::setValidLinkPreviewThickness(float thickness) { validLinkPreviewThickness = thickness; }

Color Pin::getInvalidLinkPreviewColor() const { return invalidLinkPreviewColor; }

void Pin::setInvalidLinkPreviewColor(Color color) { invalidLinkPreviewColor = color; }

float Pin::getInvalidLinkPreviewThickness() const { return invalidLinkPreviewThickness; }

void Pin::setInvalidLinkPreviewThickness(float thickness) { invalidLinkPreviewThickness = thickness; }

Color Pin::getUnconnectedLinkPreviewColor() const { return unconnectedLinkPreviewColor; }

void Pin::setUnconnectedLinkPreviewColor(Color color) { unconnectedLinkPreviewColor = color; }

float Pin::getUnconnectedLinkPreviewThickness() const { return unconnectedLinkPreviewThickness; }

void Pin::setUnconnectedLinkPreviewThickness(float thickness) { unconnectedLinkPreviewThickness = thickness; }

ranges::transform_view<ranges::ref_view<std::vector<std::unique_ptr<pf::ui::ig::Link>>>, details::LinkPtrToRef>
Pin::getAllLinks() {
  return getNode().getNodeEditor().getLinks();
}

ranges::transform_view<ranges::ref_view<const std::vector<std::unique_ptr<pf::ui::ig::Link>>>,
                       details::LinkPtrToConstRef>

Pin::getAllLinks() const {
  return getNode().getNodeEditor().getLinks();
}

void Pin::setHovered(bool newHovered) { *Prop_modify(hovered) = newHovered; }

}  // namespace pf::ui::ig