/**
* @file NodeEditor.h
* @brief Node editor management class.
* @author Petr Flajšingr
* @date 27.3.22
*/

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
   * Create a new node of given type.
   * TODO: node position setter
   * @tparam T type of the node to create
   * @tparam Args types of arguments for Node's construction
   * @param args arguments for Node's construction
   * @return reference to the newly created node
   */
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

  /**
   * Register a callable which is invoked when a request for new node creation occurs from user.
   * This request happens when the user extends a link from a pin into open space.
   *
   * The NodeEditor will become suspended, you either need to call NodeEditor::resume after you're done modifying it
   * or you can return true from the handler to let the editor resume itself.
   * @param handler callback for request
   */
  void setCreateNodeRequestHandler(std::invocable<Pin &> auto &&handler)
    requires std::same_as<bool, std::invoke_result_t<decltype(handler), Pin &>>
  {
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
   *
   * @return next id for internal elements
   */
  [[nodiscard]] int getNextId();

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
