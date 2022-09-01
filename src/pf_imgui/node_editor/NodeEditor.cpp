//
// Created by xflajs00 on 27.03.2022.
//

#include "NodeEditor.h"
#include "Link.h"
#include "Node.h"
#include "Pin.h"
#include <pf_common/RAII.h>
#include <pf_imgui/unique_id.h>
#include <range/v3/view/cache1.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

NodeEditor::NodeEditor(const std::string &elementName, Size initialSize)
    : ElementWithID(elementName), Resizable(initialSize), context(ax::NodeEditor::CreateEditor()) {}

NodeEditor::NodeEditor(NodeEditor::Config &&config)
    : ElementWithID(std::move(config.name)), Resizable(config.size), context(ax::NodeEditor::CreateEditor()) {}

NodeEditor::~NodeEditor() { ax::NodeEditor::DestroyEditor(context); }

void NodeEditor::renderImpl() {
  {
    setContext();
    ax::NodeEditor::Begin(getName().c_str(), static_cast<ImVec2>(getSize()));
    auto end = RAII{ax::NodeEditor::End};
    {
      std::ranges::for_each(nodes, [](auto &node) { node->render(); });
      std::ranges::for_each(comments, [](auto &comment) { comment->render(); });
      std::ranges::for_each(links, [](auto &link) { link->render(); });

      handleCreation();
      handleDeletion();
      handleSelectionChange();

      handlePopupMenuShowRequests();
    }
  }
  handleClickEvents();
  handleHoverEvents();
  renderPopupMenuForRequested();
}

std::optional<Node *> NodeEditor::findNodeById(ax::NodeEditor::NodeId nodeId) {
  const auto getNodeId = [](auto &node) { return node->id; };
  if (auto iter = std::ranges::find(nodes, nodeId, getNodeId); iter != nodes.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Comment *> NodeEditor::findCommentById(ax::NodeEditor::NodeId nodeId) {
  const auto getCommentId = [](auto &comment) { return comment->id; };
  if (auto iter = std::ranges::find(comments, nodeId, getCommentId); iter != comments.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Pin *> NodeEditor::findPinById(ax::NodeEditor::PinId pinId) {
  const auto getPinId = [](auto &pin) { return pin->id; };
  auto pins = nodes
      | ranges::views::transform([](auto &node) { return ranges::views::concat(node->inputPins, node->outputPins); })
      | ranges::views::cache1 | ranges::views::join;
  if (auto iter = std::ranges::find(pins, pinId, getPinId); iter != pins.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Link *> NodeEditor::findLinkById(ax::NodeEditor::LinkId linkId) {
  const auto getLinkId = [](auto &link) { return link->id; };
  if (auto iter = std::ranges::find(links, linkId, getLinkId); iter != links.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Node *> NodeEditor::findNodeByName(const std::string &nodeName) {
  const auto getNodeName = [](auto &node) { return node->getName(); };
  if (auto iter = std::ranges::find(nodes, nodeName, getNodeName); iter != nodes.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Comment *> NodeEditor::findCommentByName(const std::string &nodeName) {
  const auto getCommentName = [](auto &comment) { return comment->getName(); };
  if (auto iter = std::ranges::find(comments, nodeName, getCommentName); iter != comments.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Pin *> NodeEditor::findPinByName(const std::string &pinName) {
  const auto getPinName = [](auto &pin) { return pin->getName(); };
  auto pins = nodes
      | ranges::views::transform([](auto &node) { return ranges::views::concat(node->inputPins, node->outputPins); })
      | ranges::views::cache1 | ranges::views::join;
  if (auto iter = std::ranges::find(pins, pinName, getPinName); iter != pins.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Link *> NodeEditor::findLinkByName(const std::string &linkName) {
  const auto getLinkName = [](auto &link) { return link->getName(); };
  if (auto iter = std::ranges::find(links, linkName, getLinkName); iter != links.end()) { return iter->get(); }
  return std::nullopt;
}

void NodeEditor::handleCreation() {
  [[maybe_unused]] auto endCreate = RAII{ax::NodeEditor::EndCreate};
  if (ax::NodeEditor::BeginCreate()) {
    handleLinkCreation();
    handleNodeCreation();
  }
}

void NodeEditor::handleLinkCreation() {
  const static auto rejectLink = [](Pin &originPin) {
    ax::NodeEditor::RejectNewItem(static_cast<ImVec4>(originPin.getInvalidLinkPreviewColor()),
                                  originPin.getInvalidLinkPreviewThickness());
  };
  ax::NodeEditor::PinId inPinId;
  ax::NodeEditor::PinId outPinId;
  if (ax::NodeEditor::QueryNewLink(&inPinId, &outPinId)) {
    if (inPinId && outPinId) {
      auto inPinOpt = findPinById(inPinId);
      auto outPinOpt = findPinById(outPinId);

      if (inPinOpt.has_value() && outPinOpt.has_value()) {
        auto inPin = *inPinOpt;
        auto outPin = *outPinOpt;

        if (!inPin->acceptsNewLinks() || !outPin->acceptsNewLinks()) {
          rejectLink(*inPin);
          return;
        }
        if (!inPin->acceptsLinkWith(*outPin) || !outPin->acceptsLinkWith(*inPin)) {
          rejectLink(*inPin);
          return;
        }

        if (ax::NodeEditor::AcceptNewItem(static_cast<ImVec4>(inPin->getValidLinkPreviewColor()),
                                          inPin->getValidLinkPreviewThickness())) {
          if (inPin->getType() == Pin::Type::Output && outPin->getType() == Pin::Type::Input) {
            std::swap(inPin, outPin);
          }
          addLink(uniqueId(), *inPin, *outPin);
        }
      }
    }
  }
}

void NodeEditor::handleNodeCreation() {
  ax::NodeEditor::PinId pinId;
  if (ax::NodeEditor::QueryNewNode(&pinId)) {
    const auto pinOpt = findPinById(pinId);
    if (pinOpt.has_value()) {
      auto pin = *pinOpt;
      if (ax::NodeEditor::AcceptNewItem(static_cast<ImVec4>(pin->getUnconnectedLinkPreviewColor()),
                                        pin->getUnconnectedLinkPreviewThickness())) {
        createNodeRequestHandler(*pin);
      }
    }
  }
}

void NodeEditor::handleDeletion() {
  [[maybe_unused]] auto endDelete = RAII{ax::NodeEditor::EndDelete};
  if (ax::NodeEditor::BeginDelete()) {
    handleLinkDeletion();
    handleNodeDeletion();
  }
  removeMarkedElements();
}

void NodeEditor::handleLinkDeletion() {
  ax::NodeEditor::LinkId deletedLinkId;
  while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId)) {
    if (ax::NodeEditor::AcceptDeletedItem()) {
      const auto getLinkId = [](auto &link) { return link->id; };
      auto iter = std::ranges::find(links, deletedLinkId, getLinkId);
      if (iter != links.end()) {
        iter->get()->invalidate();
        linksDirty = true;
      }
    }
  }
}

void NodeEditor::handleNodeDeletion() {
  ax::NodeEditor::NodeId nodeId;
  while (ax::NodeEditor::QueryDeletedNode(&nodeId)) {
    if (ax::NodeEditor::AcceptDeletedItem()) {
      if (const auto nodeToDelete = findNodeById(nodeId); nodeToDelete.has_value()) {
        (*nodeToDelete)->markedForDelete = true;
        markNodesDirty();
      } else if (const auto commentToDelete = findCommentById(nodeId); commentToDelete.has_value()) {
        (*commentToDelete)->markedForDelete = true;
        markNodesDirty();
      }
    }
  }
}

void NodeEditor::removeMarkedElements() {
  if (linksDirty) {
    std::ranges::for_each(getLinks() | ranges::views::filter([](const auto &link) { return !link.isValid(); }),
                          [](auto &link) {
                            link.observableDelete.notify();
                            if (link.inputPin != nullptr) { link.getInputPin().observableLink.notify(link); }
                            if (link.outputPin != nullptr) { link.getOutputPin().observableLink.notify(link); }
                          });
    auto [beginRm, endRm] = std::ranges::remove_if(links, [](const auto &link) { return !link->isValid(); });
    links.erase(beginRm, endRm);
    linksDirty = false;
  }

  if (nodesDirty) {
    {
      auto markedNodes = getNodes() | ranges::views::filter([](const auto &node) { return node.markedForDelete; });
      auto inputPins =
          markedNodes | ranges::views::transform([](auto &node) { return node.getInputPins(); }) | ranges::views::join;
      std::ranges::for_each(inputPins, [](auto &pin) {
        std::ranges::for_each(pin.getLinks(), [](auto &link) {
          link.invalidate();
          link.inputPin = nullptr;
        });
      });
      auto outputPins =
          markedNodes | ranges::views::transform([](auto &node) { return node.getOutputPins(); }) | ranges::views::join;
      std::ranges::for_each(outputPins, [](auto &pin) {
        std::ranges::for_each(pin.getLinks(), [](auto &link) {
          link.invalidate();
          link.outputPin = nullptr;
        });
      });

      std::ranges::for_each(markedNodes, [](auto &node) { node.observableDelete.notify(); });
      auto [beginRm, endRm] = std::ranges::remove_if(nodes, [](const auto &node) { return node->markedForDelete; });
      nodes.erase(beginRm, endRm);
    }
    {
      auto markedComments =
          getComments() | ranges::views::filter([](const auto &node) { return node.markedForDelete; });
      std::ranges::for_each(markedComments, [](auto &comment) { comment.observableDelete.notify(); });
      auto [beginRm, endRm] =
          std::ranges::remove_if(comments, [](const auto &comment) { return comment->markedForDelete; });
      comments.erase(beginRm, endRm);
    }
    nodesDirty = false;
  }
}

void NodeEditor::handleSelectionChange() {
  if (ax::NodeEditor::HasSelectionChanged()) {
    const auto selectedObjectCount = ax::NodeEditor::GetSelectedObjectCount();
    {
      auto selectedNodeIds = std::vector<ax::NodeEditor::NodeId>(selectedObjectCount);
      selectedNodeIds.resize(ax::NodeEditor::GetSelectedNodes(selectedNodeIds.data(), selectedObjectCount));

      std::ranges::for_each(nodes | ranges::views::filter([&](const auto &node) {
                              const auto isSelected =
                                  std::ranges::find(selectedNodeIds, node->getId()) != selectedNodeIds.end();
                              const auto wasSelected = node->isSelected();
                              return isSelected != wasSelected;
                            }),
                            [](const auto &node) {
                              node->selected = !node->selected;
                              node->observableSelected.notify(node->selected);
                            });
    }
    {
      auto selectedLinkIds = std::vector<ax::NodeEditor::LinkId>(selectedObjectCount);
      selectedLinkIds.resize(ax::NodeEditor::GetSelectedLinks(selectedLinkIds.data(), selectedObjectCount));

      std::ranges::for_each(links | ranges::views::filter([&](const auto &link) {
                              const auto isSelected =
                                  std::ranges::find(selectedLinkIds, link->getId()) != selectedLinkIds.end();
                              const auto wasSelected = link->isSelected();
                              return isSelected != wasSelected;
                            }),
                            [](const auto &link) {
                              link->selected = !link->selected;
                              link->observableSelected.notify(link->selected);
                            });
    }
  }
}

bool NodeEditor::isSuspended() const {
  setContext();
  return ax::NodeEditor::IsSuspended();
}

void NodeEditor::suspend() {
  setContext();
  ax::NodeEditor::Suspend();
}

void NodeEditor::resume() {
  setContext();
  ax::NodeEditor::Resume();
}

Link &NodeEditor::addLink(std::string linkName, Pin &inputPin, Pin &outputPin) {
  auto &newLink = links.emplace_back(std::make_unique<Link>(linkName, getNextId(), &inputPin, &outputPin));
  inputPin.addLink(*newLink);
  outputPin.addLink(*newLink);
  return *newLink;
}

void NodeEditor::clearSelection() {
  setContext();
  ax::NodeEditor::ClearSelection();
}

void NodeEditor::navigateToContent(std::optional<std::chrono::milliseconds> animationLength) {
  setContext();
  ax::NodeEditor::NavigateToContent(
      static_cast<float>(animationLength.value_or(std::chrono::milliseconds{-1000}).count()) / 1000.f);
}

void NodeEditor::navigateToSelection(bool zoomIn, std::optional<std::chrono::milliseconds> animationLength) {
  setContext();
  ax::NodeEditor::NavigateToSelection(
      zoomIn, static_cast<float>(animationLength.value_or(std::chrono::milliseconds{-1000}).count()) / 1000.f);
}

PopupMenu &NodeEditor::createOrGetPopupMenu() {
  if (popupMenu == nullptr) { popupMenu = std::make_unique<PopupMenu>("popup"); }
  return *popupMenu;
}

bool NodeEditor::hasPopupMenu() const { return popupMenu != nullptr; }

void NodeEditor::removePopupMenu() { popupMenu = nullptr; }

int NodeEditor::getNextId() { return idCounter++; }

void NodeEditor::markLinksDirty() { linksDirty = true; }

void NodeEditor::markNodesDirty() { nodesDirty = true; }

void NodeEditor::setContext() const { ax::NodeEditor::SetCurrentEditor(context); }

void NodeEditor::handlePopupMenuShowRequests() {
  ax::NodeEditor::NodeId contextNodeId;
  ax::NodeEditor::PinId contextPinId;
  ax::NodeEditor::LinkId contextLinkId;
  if (ax::NodeEditor::ShowNodeContextMenu(&contextNodeId)) {
    if (auto nodeOpt = findNodeById(contextNodeId); nodeOpt.has_value()) {
      if ((*nodeOpt)->hasPopupMenu()) {
        popupPtrs.node = *nodeOpt;
        popupPtrs.node->popupMenu->open();
      }
    } else if (auto commentOpt = findCommentById(contextNodeId); commentOpt.has_value()) {
      if ((*commentOpt)->hasPopupMenu()) {
        popupPtrs.node = *commentOpt;
        popupPtrs.node->popupMenu->open();
      }
    }
  } else if (ax::NodeEditor::ShowPinContextMenu(&contextPinId)) {
    if (auto pinOpt = findPinById(contextPinId); pinOpt.has_value()) {
      if ((*pinOpt)->hasPopupMenu()) {
        popupPtrs.pin = *pinOpt;
        popupPtrs.pin->popupMenu->open();
      }
    }
  } else if (ax::NodeEditor::ShowLinkContextMenu(&contextLinkId)) {
    if (auto linkOpt = findLinkById(contextLinkId); linkOpt.has_value()) {
      if ((*linkOpt)->hasPopupMenu()) {
        popupPtrs.link = *linkOpt;
        popupPtrs.link->popupMenu->open();
      }
    }
  } else if (ax::NodeEditor::ShowBackgroundContextMenu()) {
    popupPtrs.editor = this;
  }
}

void NodeEditor::renderPopupMenuForRequested() {
  if (popupPtrs.node != nullptr) {
    if (popupPtrs.node->popupMenu == nullptr || !popupPtrs.node->popupMenu->isOpen()) {
      popupPtrs.node = nullptr;
    } else {
      popupPtrs.node->popupMenu->render();
    }
  }
  if (popupPtrs.pin != nullptr) {
    if (popupPtrs.pin->popupMenu == nullptr || !popupPtrs.pin->popupMenu->isOpen()) {
      popupPtrs.pin = nullptr;
    } else {
      popupPtrs.pin->popupMenu->render();
    }
  }
  if (popupPtrs.link != nullptr) {
    if (popupPtrs.link->popupMenu == nullptr || !popupPtrs.link->popupMenu->isOpen()) {
      popupPtrs.link = nullptr;
    } else {
      popupPtrs.link->popupMenu->render();
    }
  }
  if (popupPtrs.editor != nullptr) {
    if (popupPtrs.editor->popupMenu == nullptr || !popupPtrs.editor->popupMenu->isOpen()) {
      popupPtrs.editor = nullptr;
    } else {
      popupPtrs.editor->popupMenu->render();
    }
  }
}

void NodeEditor::handleClickEvents() {
  setContext();
  const auto doubleClickedNode = ax::NodeEditor::GetDoubleClickedNode();
  const auto doubleClickedPin = ax::NodeEditor::GetDoubleClickedPin();
  const auto doubleClickedLink = ax::NodeEditor::GetDoubleClickedLink();
  if (doubleClickedNode.Get() != 0) {
    if (auto node = findNodeById(doubleClickedNode); node.has_value()) {
      (*node)->observableDoubleClick.notify();
    } else if (auto comment = findCommentById(doubleClickedNode); comment.has_value()) {
      (*comment)->observableDoubleClick.notify();
    }
  }
  if (doubleClickedPin.Get() != 0) {
    if (auto pin = findPinById(doubleClickedPin); pin.has_value()) { (*pin)->observableDoubleClick.notify(); }
  }
  if (doubleClickedLink.Get() != 0) {
    if (auto link = findLinkById(doubleClickedLink); link.has_value()) { (*link)->observableDoubleClick.notify(); }
  }
  if (ax::NodeEditor::IsBackgroundClicked()) { observableBackgroundClick.notify(); }
  if (ax::NodeEditor::IsBackgroundDoubleClicked()) { observableBackgroundDoubleClick.notify(); }
}

void NodeEditor::handleHoverEvents() {
  setContext();
  const auto hoveredNode = ax::NodeEditor::GetHoveredNode();
  const auto hoveredPin = ax::NodeEditor::GetHoveredPin();
  const auto hoveredLink = ax::NodeEditor::GetHoveredLink();

  if (hoveredNode != hoverIds.node) {
    if (hoverIds.node.Get() != 0) {
      if (auto node = findNodeById(hoverIds.node); node.has_value()) {
        (*node)->setHovered(false);
      } else if (auto comment = findCommentById(hoverIds.node); comment.has_value()) {
        (*comment)->setHovered(false);
      }
    }
    if (auto node = findNodeById(hoveredNode); node.has_value()) {
      (*node)->setHovered(true);
    } else if (auto comment = findCommentById(hoveredNode); comment.has_value()) {
      (*comment)->setHovered(true);
    }
    hoverIds.node = hoveredNode;
  }
  if (hoveredPin != hoverIds.pin) {
    if (hoverIds.pin.Get() != 0) {
      if (auto pin = findPinById(hoverIds.pin); pin.has_value()) { (*pin)->setHovered(false); }
    }
    if (auto pin = findPinById(hoveredPin); pin.has_value()) { (*pin)->setHovered(true); }
    hoverIds.pin = hoveredPin;
  }
  if (hoveredLink != hoverIds.link) {
    if (hoverIds.link.Get() != 0) {
      if (auto link = findLinkById(hoverIds.link); link.has_value()) { (*link)->setHovered(false); }
    }
    if (auto link = findLinkById(hoveredLink); link.has_value()) { (*link)->setHovered(true); }
    hoverIds.link = hoveredLink;
  }
}
}  // namespace pf::ui::ig