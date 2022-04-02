//
// Created by xflajs00 on 27.03.2022.
//

#include "NodeEditor.h"
#include "Link.h"
#include "Node.h"
#include "Pin.h"
#include "pf_common/RAII.h"
#include "pf_imgui/unique_id.h"
#include <range/v3/view/cache1.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

NodeEditor::NodeEditor(const std::string &name, Size size)
    : Element(name), Resizable(size), context(ax::NodeEditor::CreateEditor()) {}

NodeEditor::NodeEditor(NodeEditor::Config &&config)
    : Element(std::move(config.name)), Resizable(config.size), context(ax::NodeEditor::CreateEditor()) {}

NodeEditor::~NodeEditor() { ax::NodeEditor::DestroyEditor(context); }

void NodeEditor::renderImpl() {
  {
    auto scopedContext = setContext();
    ax::NodeEditor::Begin(getName().c_str(), getSize().asImVec());
    auto end = RAII{ax::NodeEditor::End};
    {
      std::ranges::for_each(nodes, [](auto &node) { node->render(); });
      std::ranges::for_each(links, [](auto &link) { link->render(); });

      handleCreation();
      handleDeletion();
      handleSelectionChange();

      ax::NodeEditor::NodeId contextNodeId;
      ax::NodeEditor::PinId contextPinId;
      ax::NodeEditor::LinkId contextLinkId;
      if (ax::NodeEditor::ShowNodeContextMenu(&contextNodeId)) {
        if (auto nodeOpt = findNodeById(contextNodeId); nodeOpt.has_value()) {
          if (nodeOpt.value()->hasPopupMenu()) {
            popupPtrs.node = nodeOpt.value();
            popupPtrs.node->popupMenu->open();
          }
        }
      } else if (ax::NodeEditor::ShowPinContextMenu(&contextPinId)) {
        if (auto pinOpt = findPinById(contextPinId); pinOpt.has_value()) {
          if (pinOpt.value()->hasPopupMenu()) {
            popupPtrs.pin = pinOpt.value();
            popupPtrs.pin->popupMenu->open();
          }
        }
      } else if (ax::NodeEditor::ShowLinkContextMenu(&contextLinkId)) {
        if (auto linkOpt = findLinkById(contextLinkId); linkOpt.has_value()) {
          if (linkOpt.value()->hasPopupMenu()) {
            popupPtrs.link = linkOpt.value();
            popupPtrs.link->popupMenu->open();
          }
        }
      }
    }
  }

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
}

std::optional<Node *> NodeEditor::findNodeById(ax::NodeEditor::NodeId id) {
  const auto getNodeId = [](auto &node) { return node->id; };
  if (auto iter = std::ranges::find(nodes, id, getNodeId); iter != nodes.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Pin *> NodeEditor::findPinById(ax::NodeEditor::PinId id) {
  const auto getPinId = [](auto &pin) { return pin->id; };
  auto pins = nodes
      | ranges::views::transform([](auto &node) { return ranges::views::concat(node->inputPins, node->outputPins); })
      | ranges::views::cache1 | ranges::views::join;
  if (auto iter = std::ranges::find(pins, id, getPinId); iter != pins.end()) { return iter->get(); }
  return std::nullopt;
}

std::optional<Link *> NodeEditor::findLinkById(ax::NodeEditor::LinkId id) {
  const auto getLinkId = [](auto &link) { return link->id; };
  if (auto iter = std::ranges::find(links, id, getLinkId); iter != links.end()) { return iter->get(); }
  return std::nullopt;
}

void NodeEditor::handleCreation() {
  auto endCreate = RAII{ax::NodeEditor::EndCreate};
  if (ax::NodeEditor::BeginCreate()) {
    handleLinkCreation();
    handleNodeCreation();
  }
}

void NodeEditor::handleLinkCreation() {
  const static auto rejectLink = [](Pin &originPin) {
    ax::NodeEditor::RejectNewItem(originPin.getInvalidLinkPreviewColor(), originPin.getInvalidLinkPreviewThickness());
  };
  ax::NodeEditor::PinId inPinId;
  ax::NodeEditor::PinId outPinId;
  if (ax::NodeEditor::QueryNewLink(&inPinId, &outPinId)) {
    if (inPinId && outPinId) {
      auto inPinOpt = findPinById(inPinId);
      auto outPinOpt = findPinById(outPinId);

      if (inPinOpt.has_value() && outPinOpt.has_value()) {
        auto inPin = inPinOpt.value();
        auto outPin = outPinOpt.value();

        if (!inPin->acceptsNewLinks() || !outPin->acceptsNewLinks()) {
          rejectLink(*inPin);
          return;
        }
        if (!inPin->acceptsLinkWith(*outPin) || !outPin->acceptsLinkWith(*inPin)) {
          rejectLink(*inPin);
          return;
        }

        if (ax::NodeEditor::AcceptNewItem(inPin->getValidLinkPreviewColor(), inPin->getValidLinkPreviewThickness())) {
          if (inPin->getType() == Pin::Type::Output && outPin->getType() == Pin::Type::Input) {
            std::swap(inPin, outPin);
          }
          auto &newLink = links.emplace_back(std::make_unique<Link>(uniqueId(), getNextId(), inPin, outPin));
          inPin->addLink(*newLink);
          outPin->addLink(*newLink);
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
      auto pin = pinOpt.value();
      if (ax::NodeEditor::AcceptNewItem(pin->getUnconnectedLinkPreviewColor(),
                                        pin->getUnconnectedLinkPreviewThickness())) {
        createNodeRequestHandler(*pin);
      }
    }
  }
}

void NodeEditor::handleDeletion() {
  auto endDelete = RAII{ax::NodeEditor::EndDelete};
  if (ax::NodeEditor::BeginDelete()) {
    handleLinkDeletion();
    handleNodeDeletion();
  }
}

void NodeEditor::handleLinkDeletion() {
  ax::NodeEditor::LinkId deletedLinkId;
  while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId)) {
    if (ax::NodeEditor::AcceptDeletedItem()) {
      const auto getLinkId = [](auto &link) { return link->id; };
      auto iter = std::ranges::find(links, deletedLinkId, getLinkId);
      if (iter != links.end()) {
        iter->get()->invalidate();

        iter->get()->getInputPin().observableLink.notify(**iter);
        iter->get()->getOutputPin().observableLink.notify(**iter);

        links.erase(iter);
      }
    }
  }
  if (linksDirty) {
    auto [beginRm, endRm] = std::ranges::remove_if(links, [](const auto &link) { return !link->isValid(); });
    links.erase(beginRm, endRm);
    linksDirty = false;
  }
}

void NodeEditor::handleNodeDeletion() {
  ax::NodeEditor::NodeId nodeId;
  while (ax::NodeEditor::QueryDeletedNode(&nodeId)) {
    if (ax::NodeEditor::AcceptDeletedItem()) {
      auto [rmBegin, rmEnd] = std::ranges::remove(nodes, nodeId, [](auto &node) { return node->getId(); });
      nodes.erase(rmBegin, rmEnd);
    }
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
                              return (isSelected && !wasSelected) || (!isSelected && wasSelected);
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
                              return (isSelected && !wasSelected) || (!isSelected && wasSelected);
                            }),
                            [](const auto &link) {
                              link->selected = !link->selected;
                              link->observableSelected.notify(link->selected);
                            });
    }
  }
}

bool NodeEditor::isSuspended() const {
  auto scopedContext = setContext();
  return ax::NodeEditor::IsSuspended();
}

void NodeEditor::suspend() {
  auto scopedContext = setContext();
  ax::NodeEditor::Suspend();
}

void NodeEditor::resume() {
  auto scopedContext = setContext();
  ax::NodeEditor::Resume();
}

void NodeEditor::clearSelection() {
  auto scopedContext = setContext();
  ax::NodeEditor::ClearSelection();
}

void NodeEditor::navigateToContent(std::optional<std::chrono::milliseconds> animationLength) {
  auto scopedContext = setContext();
  ax::NodeEditor::NavigateToContent(
      static_cast<float>(animationLength.value_or(std::chrono::milliseconds{-1000}).count()) / 1000.f);
}

void NodeEditor::navigateToSelection(bool zoomIn, std::optional<std::chrono::milliseconds> animationLength) {
  auto scopedContext = setContext();
  ax::NodeEditor::NavigateToSelection(
      zoomIn, static_cast<float>(animationLength.value_or(std::chrono::milliseconds{-1000}).count()) / 1000.f);
}

int NodeEditor::getNextId() { return idCounter++; }

void NodeEditor::markLinksDirty() { linksDirty = true; }

RAII NodeEditor::setContext() const {
  ax::NodeEditor::SetCurrentEditor(context);
  return RAII{[] { ax::NodeEditor::SetCurrentEditor(nullptr); }};
}

}  // namespace pf::ui::ig