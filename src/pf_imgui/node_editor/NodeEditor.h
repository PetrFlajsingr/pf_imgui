//
// Created by xflajs00 on 27.03.2022.
//

#ifndef PF_IMGUI_NODE_EDITOR_NODEEDITOR_H
#define PF_IMGUI_NODE_EDITOR_NODEEDITOR_H

#include "Node.h"
#include "fwd.h"
#include <imgui_node_editor.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {
// TODO: config
// TODO: selection controls
// TODO: navigate to
// TODO: comments
class NodeEditor : public Element, public Resizable {
 public:
  struct Config {
    using Parent = NodeEditor;
    std::string name;
    Size size = Size::Auto();
  };
  NodeEditor(const std::string &name, Size size = Size::Auto());
  NodeEditor(Config &&config);
  ~NodeEditor();

  template<std::derived_from<Node> T = Node, typename... Args>
  T &addNode(Args &&...args)
    requires std::constructible_from<T, Args...>
  {
    auto newPin = std::make_unique<T>(std::forward<Args>(args)...);
    auto result = newPin.get();
    nodes.emplace_back(std::move(newPin));
    result->id = getNextId();
    result->parent = this;
    return *result;
  }

  void setCreateNodeRequestHandler(std::invocable<Pin &> auto &&handler)
    requires std::same_as<bool, std::invoke_result_t<decltype(handler), Pin &>>
  {
    createNodeRequestHandler = std::forward<decltype(handler)>(handler);
  }

  [[nodiscard]] bool isSuspended() const;
  void suspend();
  void resume();

  int getNextId();

 protected:
  void renderImpl() override;

 private:
  std::optional<Node *> findNodeById(ax::NodeEditor::NodeId id);
  std::optional<Pin *> findPinById(ax::NodeEditor::PinId id);
  std::optional<Link *> findLinkById(ax::NodeEditor::LinkId id);

  constexpr static int LINK_CLEANUP_FREQUENCY = 100;
  int sinceLastLinkCleanup = LINK_CLEANUP_FREQUENCY;

  void cleanupLinks();

  void handleCreation();
  void handleLinkCreation();
  void handleNodeCreation();

  void handleDeletion();
  void handleLinkDeletion();
  void handleNodeDeletion();

  ax::NodeEditor::EditorContext *context = nullptr;

  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::shared_ptr<Link>> links;

  std::function<bool(Pin &)> createNodeRequestHandler = [](Pin &) { return true; };

  struct {
    Node *node = nullptr;
    Pin *pin = nullptr;
    Link *link = nullptr;
  } popupPtrs;

  int idCounter = 1;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_NODEEDITOR_H
