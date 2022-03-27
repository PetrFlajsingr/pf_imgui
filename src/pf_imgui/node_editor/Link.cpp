//
// Created by xflajs00 on 27.03.2022.
//

#include "Link.h"
#include "Pin.h"

namespace pf::ui::ig {

Link::Link(const std::string &name, const ax::NodeEditor::LinkId &id, Pin *inputPin, Pin *outputPin)
    : Renderable(name), id(id), inputPin(inputPin), outputPin(outputPin) {}

Link::~Link() { valid = false; }

ax::NodeEditor::LinkId Link::getId() const { return id; }

Pin &Link::getInputPin() const { return *inputPin; }

Pin &Link::getOutputPin() const { return *outputPin; }

bool Link::isValid() const { return valid; }

void Link::invalidate() { valid = false; }

const ImVec4 &Link::getColor() const { return color; }

void Link::setColor(const ImVec4 &color) { Link::color = color; }

float Link::getThickness() const { return thickness; }

void Link::setThickness(float thickness) { Link::thickness = thickness; }

PopupMenu &Link::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>(getName() + "_popup"); }
  return *popupMenu;
}

bool Link::hasPopupMenu() const { return popupMenu != nullptr; }

void Link::removePopupMenu() { popupMenu = nullptr; }

void Link::renderImpl() {
  if (!valid) { return; }
  ax::NodeEditor::Link(id, inputPin->getId(), outputPin->getId(), color, thickness);
}
}  // namespace pf::ui::ig