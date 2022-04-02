/**
* @file NodeBase.h
* @brief Base class for node like objects in NodeEditor.
* @author Petr Flajšingr
* @date 2.4.22
*/

#ifndef PF_IMGUI_NODEBASE_H
#define PF_IMGUI_NODEBASE_H

#include "fwd.h"
#include <imgui_node_editor.h>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <pf_imgui/interface/Positionable.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {

/**
 * @brief Base class for node like objects in NodeEditor.
 */
class NodeBase : public Renderable {
  friend class NodeEditor;

 public:
  /**
   * Construct NodeBase
   * @param name unique name of the element
   */
  explicit NodeBase(const std::string &name);

  /**
   *
   * @return unique internal id in NodeEditor
   */
  [[nodiscard]] ax::NodeEditor::NodeId getId() const;

  [[nodiscard]] NodeEditor &getNodeEditor();
  [[nodiscard]] const NodeEditor &getNodeEditor() const;
  /**
   * Add a listener called when the Node is de/selected.
   * @param listener listener
   * @return Subscription for listener unsubscription
   */
  Subscription addSelectionListener(std::invocable<bool> auto &&listener) {
    return observableSelected.addListener(std::forward<decltype(listener)>(listener));
  }

  /**
   * Add a listener called when the Node is double clicked.
   * @param listener listener
   * @return Subscription for listener unsubscription
   */
  Subscription addDoubleClickListener(std::invocable auto &&listener) {
    return observableDoubleClick.addListener(std::forward<decltype(listener)>(listener));
  }

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
   * Remove the Node from NodeEditor.
   */
  void deleteNode();

  /**
   * Add a listener called when the Node is deleted.
   * @param listener listener
   * @return Subscription for listener unsubscription
   */
  Subscription addDeleteListener(std::invocable auto &&listener) {
    return observableDelete.addListener(std::forward<decltype(listener)>(listener));
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

 private:
  ax::NodeEditor::NodeId id;
  NodeEditor *parent;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;

  bool selected = false;
  Observable_impl<bool> observableSelected;
  Observable_impl<> observableDelete;
  bool markedForDelete = false;
  Observable_impl<> observableDoubleClick;
};

}  // namespace pf::ui::ig
#endif  //PF_IMGUI_NODEBASE_H
