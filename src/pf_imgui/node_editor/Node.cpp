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

Node::Node(const std::string &name) : NodeBase(name) {}

void Node::clearLinks() {
  std::ranges::for_each(ranges::views::concat(getInputPins(), getOutputPins()), [](Pin &pin) { pin.clearLinks(); });
}

void Node::renderImpl() {
  ax::NodeEditor::BeginNode(getId());
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

int Node::getNextPinId() { return getNodeEditor().getNextId(); }

}  // namespace pf::ui::ig