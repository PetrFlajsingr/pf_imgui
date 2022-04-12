/**
* @file NodeEditor.h
* @brief Node editor management class.
* @author Petr Flajšingr
* @date 27.3.22
*/

#ifndef PF_IMGUI_NODE_EDITOR_NODEEDITOR_H
#define PF_IMGUI_NODE_EDITOR_NODEEDITOR_H

#include "Comment.h"
#include "Link.h"
#include "Node.h"
#include "NodeBase.h"
#include "fwd.h"
#include <chrono>
#include <imgui_node_editor.h>
#include <optional>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/node_editor/details/LinkPtrToRef.h>
#include <range/v3/view/filter.hpp>

namespace pf::ui::ig {

// TODO: config
// TODO: shortcuts
// TODO: custom control over saving data such as node position, don't let the lib save it itself - basically make it Savable
/**
 * @brief Class allowing for node editing.
 */
class NodeEditor : public Element, public Resizable {
 public:
  /**
   * @brief Struct for construction of NodeEditor.
   */
  struct Config {
    using Parent = NodeEditor;
    std::string name;         /*!< Unique name of the element */
    Size size = Size::Auto(); /*!< Size of the element */
  };
  /**
   * Construct NodeEditor
   * @param name unique name of the element
   * @param size size of the element
   */
  explicit NodeEditor(const std::string &name, Size size = Size::Auto());
  /**
   * Construct NodeEditor
   * @param config construction args @see NodeEditor::Config
   */
  explicit NodeEditor(Config &&config);

  ~NodeEditor() override;

  /**
   * Get all nodes in editor
   * @return all nodes in editor
   */
  [[nodiscard]] auto getNodes() {
    return nodes | ranges::views::transform([](auto &node) -> Node & { return *node; });
  }
  /**
   * Get all nodes in editor
   * @return all nodes in editor
   */
  [[nodiscard]] auto getNodes() const {
    return nodes | ranges::views::transform([](auto &node) -> const Node & { return *node; });
  }

  /**
   * Get all links in editor
   * @return all links in editor
   */
  [[nodiscard]] auto getLinks() { return links | ranges::views::transform(details::LinkPtrToRef{}); }
  /**
   * Get all links in editor
   * @return all links in editor
   */
  [[nodiscard]] auto getLinks() const { return links | ranges::views::transform(details::LinkPtrToConstRef{}); }

  /**
   * Create a new node of given type.
   * @tparam T type of the node to create
   * @tparam Args types of arguments for Node's construction
   * @param args arguments for Node's construction
   * @return reference to the newly created node
   */
  template<std::derived_from<Node> T = Node, typename... Args>
  T &addNode(Args &&...args)
    requires std::constructible_from<T, Args...>
  {
    auto newNode = std::make_unique<T>(std::forward<Args>(args)...);
    auto result = newNode.get();
    nodes.emplace_back(std::move(newNode));
    result->id = getNextId();
    result->parent = this;
    return *result;
  }

  /**
   * Create a new comment of given type.
   * @tparam T type of the comment to create
   * @tparam Args types of arguments for Comment's construction
   * @param args arguments for Comment's construction
   * @return reference to the newly created comment
   */
  template<std::derived_from<Comment> T = Comment, typename... Args>
  T &addComment(Args &&...args)
    requires std::constructible_from<T, Args...>
  {
    auto newComment = std::make_unique<T>(std::forward<Args>(args)...);
    auto result = newComment.get();
    comments.emplace_back(std::move(newComment));
    result->id = getNextId();
    result->parent = this;
    return *result;
  }

  /**
   * Register a callable which is invoked when a request for new node creation occurs from user.
   * This request happens when the user extends a link from a pin into open space.
   * @param handler callback for request
   */
  void setCreateNodeRequestHandler(std::invocable<Pin &> auto &&handler) {
    createNodeRequestHandler = std::forward<decltype(handler)>(handler);
  }

  /**
   * Check if the editor is in suspended state - not changing how it looks and not accepting any user interaction.
   * @return true if suspended, false otherwise
   */
  [[nodiscard]] bool isSuspended() const;
  /**
   * Suspend the editor - rendering freeze and ignore of any user interaction.
   */
  void suspend();
  /**
   * Resume the editor from suspended state.
   */
  void resume();

  /**
   * Get all nodes currently selected by user.
   * @return view to all selected nodes
   */
  [[nodiscard]] auto getSelectedNodes() {
    return nodes | ranges::views::filter([](const auto &node) { return node->isSelected(); })
        | ranges::views::transform([](const auto &node) -> Node & { return *node; });
  }
  /**
   * Get all nodes currently selected by user.
   * @return view to all selected nodes
   */
  [[nodiscard]] auto getSelectedNodes() const {
    return nodes | ranges::views::filter([](const auto &node) { return node->isSelected(); })
        | ranges::views::transform([](const auto &node) -> const Node & { return *node; });
  }

  /**
   * Get all links currently selected by user.
   * @return view to all selected links
   */
  [[nodiscard]] auto getSelectedLinks() {
    return links | ranges::views::filter([](const auto &link) { return link->isSelected(); })
        | ranges::views::transform([](const auto &link) -> Link & { return *link; });
  }
  /**
   * Get all links currently selected by user.
   * @return view to all selected links
   */
  [[nodiscard]] auto getSelectedLinks() const {
    return links | ranges::views::filter([](const auto &link) { return link->isSelected(); })
        | ranges::views::transform([](const auto &link) -> const Link & { return *link; });
  }

  /**
   * Remove all Links and Nodes from selection.
   */
  void clearSelection();

  /**
   * Zoom the window so the entire content is visible.
   * @param animationLength optional length of zoom animation
   */
  void navigateToContent(std::optional<std::chrono::milliseconds> animationLength = std::nullopt);
  /**
   * Zoom the window so selected content.
   * @param zooMin if true the function will zoom in to the selection, otherwise it will only zoom out
   * @param animationLength optional length of zoom animation
   */
  void navigateToSelection(bool zoomIn = true, std::optional<std::chrono::milliseconds> animationLength = std::nullopt);

  /**
   * Add a listener called when editor's background is clicked.
   * @param listener listener
   * @return Subscription for listener unsubscription
   */
  Subscription addBackgroundClickListener(std::invocable auto &&listener) {
    return observableBackgroundClick.addListener(std::forward<decltype(listener)>(listener));
  }
  /**
   * Add a listener called when editor's background is double clicked.
   * @param listener listener
   * @return Subscription for listener unsubscription
   */
  Subscription addBackgroundDoubleClickListener(std::invocable auto &&listener) {
    return observableBackgroundDoubleClick.addListener(std::forward<decltype(listener)>(listener));
  }

  /**
   * Create or get PopupMenu which is shown when the node is right clicked.
   */
  [[nodiscard]] PopupMenu &createOrGetPopupMenu();
  /**
   *
   * @return true if PopupMenu is active for this node, faalse otherwise
   */
  [[nodiscard]] bool hasPopupMenu() const;
  /**
   * Remove PopupMenu of this node, no lon
   */
  void removePopupMenu();

  /**
   *
   * @return next id for internal elements
   */
  [[nodiscard]] int getNextId();

  void markLinksDirty();
  void markNodesDirty();

  [[nodiscard]] RAII setContext() const;

 protected:
  void renderImpl() override;

 private:
  std::optional<Node *> findNodeById(ax::NodeEditor::NodeId nodeId);
  std::optional<Comment *> findCommentById(ax::NodeEditor::NodeId nodeId);
  std::optional<Pin *> findPinById(ax::NodeEditor::PinId pinId);
  std::optional<Link *> findLinkById(ax::NodeEditor::LinkId linkId);

  void handleCreation();
  void handleLinkCreation();
  void handleNodeCreation();

  void handleDeletion();
  void handleLinkDeletion();
  void handleNodeDeletion();

  void handleSelectionChange();

  void handlePopupMenuShowRequests();
  void renderPopupMenuForRequested();

  void handleClickEvents();
  void handleHoverEvents();

  ax::NodeEditor::EditorContext *context = nullptr;

  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::unique_ptr<Link>> links;
  std::vector<std::unique_ptr<Comment>> comments;

  std::function<void(Pin &)> createNodeRequestHandler = [](Pin &) {};

  struct {
    NodeBase *node = nullptr;
    Pin *pin = nullptr;
    Link *link = nullptr;
    NodeEditor *editor = nullptr;
  } popupPtrs;

  struct {
    ax::NodeEditor::NodeId node = 0;
    ax::NodeEditor::PinId pin = 0;
    ax::NodeEditor::LinkId link = 0;
  } hoverIds;

  int idCounter = 1;

  bool linksDirty = false;
  bool nodesDirty = false;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;
  Observable_impl<> observableBackgroundClick;
  Observable_impl<> observableBackgroundDoubleClick;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_NODEEDITOR_H
