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

Node::Node(const std::string &name) : Renderable(name) {}

Node::~Node() {
  auto links = ranges::views::concat(inputPins, outputPins)
      | ranges::view::transform([](auto &pin) { return pin->getLinks(); }) | ranges::view::cache1 | ranges::view::join;
  std::ranges::for_each(links, [](auto &link) { link->invalidate(); });
}

ax::NodeEditor::NodeId Node::getId() const { return id; }

const std::vector<std::unique_ptr<Pin>> &Node::getInputPins() const { return inputPins; }

const std::vector<std::unique_ptr<Pin>> &Node::getOutputPins() const { return outputPins; }

PopupMenu &Node::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>(getName() + "_popup"); }
  return *popupMenu;
}

bool Node::hasPopupMenu() const { return popupMenu != nullptr; }

void Node::removePopupMenu() { popupMenu = nullptr; }

void Node::renderImpl() {
  ax::NodeEditor::BeginNode(id);
  auto endNode = RAII{ax::NodeEditor::EndNode};

  ImGui::BeginGroup();
  std::ranges::for_each(inputPins, [](auto &pin) { pin->render(); });
  ImGui::EndGroup();

  ImGui::SameLine();

  ImGui::BeginGroup();
  std::ranges::for_each(outputPins, [](auto &pin) { pin->render(); });
  ImGui::EndGroup();
}

int Node::getNextPinId() { return parent->getNextId(); }

}  // namespace pf::ui::ig