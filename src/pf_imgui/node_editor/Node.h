/**
* @file NodeEditor.h
* @brief Node for NodeEditor.
* @author Petr Flajšingr
* @date 27.3.22
*/

#ifndef PF_IMGUI_NODE_EDITOR_NODE_H
#define PF_IMGUI_NODE_EDITOR_NODE_H

#include "Pin.h"
#include "fwd.h"
#include <imgui_node_editor.h>
#include <memory>
#include <pf_common/concepts/ranges.h>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Positionable.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief A node in NodeEditor containing Pins
 */
class Node : public Renderable {
  friend class NodeEditor;

 public:
  /**
   * @brief Struct for construction of Node.
   */
  struct Config {
    using Parent = Node;
    std::string name; /*!< Unique name of the element */
  };
  /**
   * Construct Node
   * @param config construction args @see Node::Config
   */
  explicit Node(Config &&config);
  /**
   * Construct Node
   * @param name unique name of the element
   */
  explicit Node(const std::string &name);
  ~Node() override;

  /**
   * Get id used internally.
   */
  [[nodiscard]] ax::NodeEditor::NodeId getId() const;

  /**
   * Get all input pins
   * @return view to input pins
   */
  [[nodiscard]] auto getInputPins() {
    return inputPins | ranges::views::transform([](auto &pin) -> Pin & { return *pin; });
  }
  /**
   * Get all output pins
   * @return view to output pins
   */
  [[nodiscard]] auto getOutputPins() {
    return outputPins | ranges::views::transform([](auto &pin) -> Pin & { return *pin; });
  }

  /**
   * Get all input pins
   * @return view to input pins
   */
  [[nodiscard]] auto getInputPins() const {
    return inputPins | ranges::views::transform([](auto &pin) -> const Pin & { return *pin; });
  }
  /**
   * Get all output pins
   * @return view to output pins
   */
  [[nodiscard]] auto getOutputPins() const {
    return outputPins | ranges::views::transform([](auto &pin) -> const Pin & { return *pin; });
  }

  /**
   * Create a new input pin of given type.
   * @tparam T  type of the pin to create
   * @tparam Args types of arguments for Pin's constructor
   * @param args arguments for Pin's constructor
   * @return reference to the newly created Pin
   */
  template<std::derived_from<Pin> T = Pin, typename... Args>
  T &addInputPin(Args &&...args)
    requires std::constructible_from<T, Args...>
  {
    auto newPin = std::make_unique<T>(std::forward<Args>(args)...);
    auto result = newPin.get();
    inputPins.emplace_back(std::move(newPin));
    result->id = getNextPinId();
    result->parent = this;
    result->type = Pin::Type::Input;
    return *result;
  }

  /**
   * Create a new output pin of given type.
   * @tparam T  type of the pin to create
   * @tparam Args types of arguments for Pin's constructor
   * @param args arguments for Pin's constructor
   * @return reference to the newly created Pin
   */
  template<std::derived_from<Pin> T = Pin, typename... Args>
  T &addOutputPin(Args &&...args)
    requires std::constructible_from<T, Args...>
  {
    auto newPin = std::make_unique<T>(std::forward<Args>(args)...);
    auto result = newPin.get();
    outputPins.emplace_back(std::move(newPin));
    result->id = getNextPinId();
    result->parent = this;
    result->type = Pin::Type::Output;
    return *result;
  }

  // TODO: change this in ItemElement too
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
   * Get node position within editor.
   * @return position within editor
   */
  [[nodiscard]] Position getPosition() const;
  /**
   * Set node position within editor.
   * @param position new position
   */
  void setPosition(Position position);

  /**
   * Get Node's size.
   * @return size
   */
  [[nodiscard]] Size getSize() const;

  /**
   * Center Node in the middle of the currently viewed are of editor.
   */
  void centerOnScreen();

  // TODO: implement this in NodeEditor
  /**
   * Check if the Node is selected in editor.
   * @return true if selected, false otherwise
   */
  [[nodiscard]] bool isSelected() const;
  /**
   * Mark node as selected.
   * @param appendToSelection append to the current selection
   */
  void select(bool appendToSelection = false);
  /**
   * Remove this Node from selection.
   */
  void deselect();
  /**
   * Add a listener called when the Node is de/selected.
   * @param listener listener
   * @return Subscription for listener unsubscription
   */
  Subscription addSelectionListener(std::invocable<bool> auto &&listener) {
    return observableSelected.addListener(std::forward<decltype(listener)>(listener));
  }

 protected:
  void renderImpl() override;

  /**
   * Render top part of the node.
   */
  virtual void renderHeader();
  /**
   * Render input pins on the left side of the node.
   */
  virtual void renderInputs();
  /**
   * Render space between input and output nodes.
   */
  virtual void renderMiddle();
  /**
   * Render output pins on the right side of the node.
   */
  virtual void renderOutputs();

 private:
  int getNextPinId();

  ax::NodeEditor::NodeId id;
  NodeEditor *parent;

  std::vector<std::unique_ptr<Pin>> inputPins;
  std::vector<std::unique_ptr<Pin>> outputPins;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;

  bool selected = false;
  Observable_impl<bool> observableSelected;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_NODE_H
