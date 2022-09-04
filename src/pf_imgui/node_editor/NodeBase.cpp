//
// Created by xflajs00 on 02.04.2022.
//

#include "NodeBase.h"
#include "NodeEditor.h"

namespace pf::ui::ig {

NodeBase::NodeBase(const std::string &elementName) : Renderable(elementName), position(Position{}), initSize(false) {
  position.addListener([this](Position pos) { updateNodePositionImpl(pos); });
}

NodeBase::NodeBase(const std::string &elementName, Position initPosition)
    : Renderable(elementName), position(initPosition), initSize(true) {
  position.addListener([this](Position pos) { updateNodePositionImpl(pos); });
}

ax::NodeEditor::NodeId NodeBase::getId() const { return id; }

NodeEditor &NodeBase::getNodeEditor() { return *parent; }

const NodeEditor &NodeBase::getNodeEditor() const { return *parent; }

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
  if (*visibility == Visibility::Visible) {
    if (isInitialised) { *position.modify() = Position{ax::NodeEditor::GetNodePosition(getId())}; }
    if (!*enabled) {
      ImGui::BeginDisabled();
      RAII raiiEnabled{ImGui::EndDisabled};
      renderImpl();
    } else {
      renderImpl();
    }
    if (initSize) {
      ax::NodeEditor::SetNodePosition(getId(), static_cast<ImVec2>(*position));
      initSize = false;
    }
    isInitialised = true;
  }
}

void NodeBase::setHovered(bool newHovered) { *hovered.modify() = newHovered; }

void NodeBase::updateNodePositionImpl(Position newPosition) {
  if (isInitialised) {
    parent->setContext();
    ax::NodeEditor::SetNodePosition(getId(), static_cast<ImVec2>(newPosition));
  } else {
    initSize = true;
  }
}

}  // namespace pf::ui::ig