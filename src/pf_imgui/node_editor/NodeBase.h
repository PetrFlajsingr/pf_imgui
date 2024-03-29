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
#include <pf_imgui/common/Position.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/reactive/Observable.h>
#include <pf_imgui/reactive/Observable_impl.h>

namespace pf::ui::ig {

/**
 * @brief Base class for node like objects in NodeEditor.
 */
class NodeBase : public Renderable {
  friend class NodeEditor;

 public:
  /**
   * Construct NodeBase
   * @param elementName unique name of the element
   */
  explicit NodeBase(std::string_view elementName);
  /**
   * Construct NodeBase
   * @param elementName unique name of the element
   * @param initPosition initial position
   */
  NodeBase(std::string_view elementName, Position initPosition);

  /**
   *
   * @return unique internal id in NodeEditor
   */
  [[nodiscard]] ax::NodeEditor::NodeId getId() const;

  [[nodiscard]] NodeEditor &getNodeEditor();
  [[nodiscard]] const NodeEditor &getNodeEditor() const;

  /**
   * Get Node's size.
   * @return size
   */
  [[nodiscard]] Size getNodeSize() const;

  /**
   * Center Node in the middle of the currently viewed are of editor.
   */
  void centerOnScreen();
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

  void render() override;

  ReadOnlyProperty<bool> hovered;
  Property<Position> position;
  ReadOnlyProperty<bool> selected;
  Event<> deleteEvent;
  Event<> doubleClickEvent;

 protected:
  void setHovered(bool newHovered);

  void updateNodePositionImpl(Position newPosition);

  ax::NodeEditor::NodeId id;
  NodeEditor *parent;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;

  bool markedForDelete = false;
  bool isInitialised = false;
  bool initSize;
};

}  // namespace pf::ui::ig
#endif  //PF_IMGUI_NODEBASE_H
