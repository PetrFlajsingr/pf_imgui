//
// Created by xflajs00 on 27.03.2022.
//

#include "Node.h"
#include "Link.h"
#include "NodeEditor.h"
#include "Pin.h"
#include <pf_common/ScopeExit.h>
#include <range/v3/view/cache1.hpp>
#include <range/v3/view/concat.hpp>

namespace pf::ui::ig {

Node::Node(std::string_view elementName) : NodeBase(elementName) {}

Node::Node(std::string_view elementName, Position initPosition) : NodeBase(elementName, initPosition) {}

void Node::clearLinks() {
  std::ranges::for_each(ranges::views::concat(getInputPins(), getOutputPins()), [](Pin &pin) { pin.clearLinks(); });
}

void Node::renderImpl() {
  ax::NodeEditor::BeginNode(getId());
  [[maybe_unused]] auto endNode = ScopeExit{&ax::NodeEditor::EndNode};

  ImGui::BeginVertical("header_vert");
  {
    ImGui::Spring(0);

    renderHeader();

    ImGui::Spring(0);

    ImGui::BeginHorizontal("content_hori");
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
  ImGui::BeginVertical("inputs");
  std::ranges::for_each(inputPins, [](auto &pin) { pin->render(); });
  ImGui::Spring(1);
  ImGui::EndVertical();
}

void Node::renderMiddle() {}

void Node::renderOutputs() {
  ImGui::BeginVertical("outputs");
  std::ranges::for_each(outputPins, [](auto &pin) { pin->render(); });
  ImGui::Spring(1);
  ImGui::EndVertical();
}

int Node::getNextPinId() { return getNodeEditor().getNextId(); }

}  // namespace pf::ui::ig