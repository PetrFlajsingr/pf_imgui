//
// Created by xflajs00 on 27.03.2022.
//

#include "Node.h"
#include "Link.h"
#include "NodeEditor.h"
#include "Pin.h"
#include "pf_common/RAII.h"
#include <range/v3/view/cache1.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

Node::Node(Node::Config &&config) : Renderable(std::move(config.name)) {}

Node::Node(const std::string &name) : Renderable(name) {}

Node::~Node() {
  auto links = ranges::views::concat(getInputPins(), getOutputPins())
      | ranges::views::transform([](auto &pin) { return pin.getLinks(); }) | ranges::views::cache1
      | ranges::views::join;
  std::ranges::for_each(links, [](auto &link) { link.invalidate(); });
}

ax::NodeEditor::NodeId Node::getId() const { return id; }

PopupMenu &Node::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>(getName() + "_popup"); }
  return *popupMenu;
}

bool Node::hasPopupMenu() const { return popupMenu != nullptr; }

void Node::removePopupMenu() { popupMenu = nullptr; }

Position Node::getPosition() const { return Position{ax::NodeEditor::GetNodePosition(getId())}; }

void Node::setPosition(Position position) { ax::NodeEditor::SetNodePosition(getId(), position.asImVec()); }

Size Node::getSize() const { return ax::NodeEditor::GetNodeSize(getId()); }

void Node::centerOnScreen() { ax::NodeEditor::CenterNodeOnScreen(getId()); }

bool Node::isSelected() const { return selected; }

void Node::select(bool appendToSelection) { ax::NodeEditor::SelectNode(getId(), appendToSelection); }

void Node::deselect() { ax::NodeEditor::DeselectNode(getId()); }

void Node::renderImpl() {
  ax::NodeEditor::BeginNode(id);
  auto endNode = RAII{ax::NodeEditor::EndNode};

  ImGui::BeginVertical((getName() + "_lay_hea").c_str());
  {
    ImGui::Spring(0);

    renderHeader();

    ImGui::Spring(0);

    ImGui::BeginHorizontal((getName() + "_lay_main").c_str());
    {
      renderInputs();
      ImGui::Spring(1);
      renderMiddle();
      ImGui::Spring(1);
      renderOutputs();
    }
    ImGui::EndHorizontal();
  }
  ImGui::EndVertical();
}

void Node::renderHeader() {}

void Node::renderInputs() {
  ImGui::BeginVertical((getName() + "lay_input").c_str());
  std::ranges::for_each(inputPins, [](auto &pin) { pin->render(); });
  ImGui::Spring(1);
  ImGui::EndVertical();
}

void Node::renderMiddle() {}

void Node::renderOutputs() {
  ImGui::BeginVertical((getName() + "_lay_output").c_str());
  std::ranges::for_each(outputPins, [](auto &pin) { pin->render(); });
  ImGui::Spring(1);
  ImGui::EndVertical();
}

int Node::getNextPinId() { return parent->getNextId(); }

}  // namespace pf::ui::ig