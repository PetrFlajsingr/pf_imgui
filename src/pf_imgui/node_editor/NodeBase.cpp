//
// Created by xflajs00 on 02.04.2022.
//

#include "NodeBase.h"
#include "NodeEditor.h"

namespace pf::ui::ig {

NodeBase::NodeBase(const std::string &name) : Renderable(name) {}

ax::NodeEditor::NodeId NodeBase::getId() const { return id; }

NodeEditor &NodeBase::getNodeEditor() { return *parent; }

const NodeEditor &NodeBase::getNodeEditor() const { return *parent; }

Position NodeBase::getPosition() const {
  auto context = parent->setContext();
  return Position{ax::NodeEditor::GetNodePosition(getId())};
}

void NodeBase::setPosition(Position position) {
  auto context = parent->setContext();
  ax::NodeEditor::SetNodePosition(getId(), position.asImVec());
}

Size NodeBase::getSize() const {
  auto context = parent->setContext();
  return ax::NodeEditor::GetNodeSize(getId());
}

void NodeBase::centerOnScreen() {
  auto context = parent->setContext();
  ax::NodeEditor::CenterNodeOnScreen(getId());
}

bool NodeBase::isSelected() const { return selected; }

void NodeBase::select(bool appendToSelection) {
  auto context = parent->setContext();
  ax::NodeEditor::SelectNode(getId(), appendToSelection);
}

void NodeBase::deselect() {
  auto context = parent->setContext();
  ax::NodeEditor::DeselectNode(getId());
}

void NodeBase::deleteNode() {
  markedForDelete = true;
  getNodeEditor().markNodesDirty();
}

PopupMenu &NodeBase::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>(getName() + "_popup"); }
  return *popupMenu;
}

bool NodeBase::hasPopupMenu() const { return popupMenu != nullptr; }

void NodeBase::removePopupMenu() { popupMenu = nullptr; }

}  // namespace pf::ui::ig