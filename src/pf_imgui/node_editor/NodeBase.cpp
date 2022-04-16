//
// Created by xflajs00 on 02.04.2022.
//

#include "NodeBase.h"
#include "NodeEditor.h"

namespace pf::ui::ig {

NodeBase::NodeBase(const std::string &name) : Renderable(name), Positionable(Position{}), initSize(false) {}
NodeBase::NodeBase(const std::string &name, Position initPosition)
    : Renderable(name), Positionable(initPosition), initSize(true) {}

ax::NodeEditor::NodeId NodeBase::getId() const { return id; }

NodeEditor &NodeBase::getNodeEditor() { return *parent; }

const NodeEditor &NodeBase::getNodeEditor() const { return *parent; }

void NodeBase::setPosition(Position newPosition) {
  if (isInitialised) {
    parent->setContext();
    ax::NodeEditor::SetNodePosition(getId(), static_cast<ImVec2>(newPosition));
  } else {
    initSize = true;
  }
  Positionable::setPosition(newPosition);
}

Size NodeBase::getNodeSize() const {
  parent->setContext();
  return ax::NodeEditor::GetNodeSize(getId());
}

void NodeBase::centerOnScreen() {
  parent->setContext();
  ax::NodeEditor::CenterNodeOnScreen(getId());
}

bool NodeBase::isSelected() const { return selected; }

void NodeBase::select(bool appendToSelection) {
  parent->setContext();
  ax::NodeEditor::SelectNode(getId(), appendToSelection);
}

void NodeBase::deselect() {
  parent->setContext();
  ax::NodeEditor::DeselectNode(getId());
}

void NodeBase::deleteNode() {
  markedForDelete = true;
  getNodeEditor().markNodesDirty();
}

PopupMenu &NodeBase::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>("popup"); }
  return *popupMenu;
}

bool NodeBase::hasPopupMenu() const { return popupMenu != nullptr; }

void NodeBase::removePopupMenu() { popupMenu = nullptr; }

void NodeBase::render() {
  if (getVisibility() == Visibility::Visible) {
    if (isInitialised) { setPosition(Position{ax::NodeEditor::GetNodePosition(getId())}); }
    if (getEnabled() == Enabled::No) {
      ImGui::BeginDisabled();
      RAII raiiEnabled{ImGui::EndDisabled};
      renderImpl();
    } else {
      renderImpl();
    }
    if (initSize) {
      ax::NodeEditor::SetNodePosition(getId(), static_cast<ImVec2>(getPosition()));
      initSize = false;
    }
    isInitialised = true;
  }
}

}  // namespace pf::ui::ig