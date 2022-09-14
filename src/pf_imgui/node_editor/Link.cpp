//
// Created by xflajs00 on 27.03.2022.
//

#include "Link.h"
#include "Pin.h"

namespace pf::ui::ig {

Link::Link(const std::string &elementName, const ax::NodeEditor::LinkId &linkId, Pin *inputPinPtr, Pin *outputPinPtr)
    : Renderable(elementName), id(linkId), inputPin(inputPinPtr), outputPin(outputPinPtr) {}

Link::~Link() { valid = false; }

ax::NodeEditor::LinkId Link::getId() const { return id; }

Pin &Link::getInputPin() const { return *inputPin; }

Pin &Link::getOutputPin() const { return *outputPin; }

bool Link::isValid() const { return valid; }

void Link::invalidate() { valid = false; }

Color Link::getColor() const { return color; }

void Link::setColor(Color newColor) { color = newColor; }

float Link::getThickness() const { return thickness; }

void Link::setThickness(float newThickness) { thickness = newThickness; }

PopupMenu &Link::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>("_popup"); }
  return *popupMenu;
}

bool Link::hasPopupMenu() const { return popupMenu != nullptr; }

void Link::removePopupMenu() { popupMenu = nullptr; }

bool Link::isSelected() const { return selected; }

void Link::select(bool appendToSelection) { ax::NodeEditor::SelectLink(getId(), appendToSelection); }

void Link::deselect() { ax::NodeEditor::DeselectLink(getId()); }

void Link::enableFlow() { flowEnabled = true; }

void Link::disableFlow() { flowEnabled = false; }

bool Link::isFlowEnabled() const { return flowEnabled; }

void Link::renderImpl() {
  if (!valid) { return; }
  ax::NodeEditor::Link(id, outputPin->getId(), inputPin->getId(), static_cast<ImVec4>(color), thickness);
  if (isFlowEnabled()) { ax::NodeEditor::Flow(id); }
}

void Link::setId(ax::NodeEditor::LinkId newId) { id = newId; }

void Link::setHovered(bool newHovered) { *hovered.modify() = newHovered; }

}  // namespace pf::ui::ig