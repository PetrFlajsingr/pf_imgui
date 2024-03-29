/**
 * @file Link.h
 * @brief Link for NodeEditor.
 * @author Petr Flajšingr
 * @date 27.3.22
 */

#ifndef PF_IMGUI_NODE_EDITOR_LINK_H
#define PF_IMGUI_NODE_EDITOR_LINK_H

#include "fwd.h"
#include <imgui_node_editor.h>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/reactive/Observable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Link connecting two Pins in NodeEditor.
 */
class Link : public Renderable {
  friend class NodeEditor;
  friend class Pin;

 public:
  /**
   * Construct Link
   * @param elementName unique name of the element
   * @param linkId unique internal id
   * @param inputPinPtr input pin
   * @param outputPinPtr output pin
   */
  Link(std::string_view elementName, const ax::NodeEditor::LinkId &linkId, Pin *inputPinPtr, Pin *outputPinPtr);
  ~Link() override;

  /**
   *
   * @return internal id within NodeEditor
   */
  [[nodiscard]] ax::NodeEditor::LinkId getId() const;
  /**
   *
   * @return reference to input pin
   */
  [[nodiscard]] Pin &getInputPin() const;
  /**
   *
   * @return reference to output pin
   */
  [[nodiscard]] Pin &getOutputPin() const;
  /**
   *
   * @return true if the link was not invalidated, false otherwise
   */
  [[nodiscard]] bool isValid() const;
  /**
   * Invalidate the Link, stopping its rendering and enqueue it for removal.
   */
  void invalidate();

  /**
   *
   * @return color of the Link line
   */
  [[nodiscard]] Color getColor() const;
  /**
   * Set color of the Link line
   * @param newColor new color
   */
  void setColor(Color newColor);
  /**
   *
   * @return thickness of Link line
   */
  [[nodiscard]] float getThickness() const;
  /**
   * Set thickness of the Link's line
   * @param newThickness
   */
  void setThickness(float newThickness);

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
   * Mark Link as selected.
   * @param appendToSelection append to the current selection
   */
  void select(bool appendToSelection = false);
  /**
   * Remove this Link from selection.
   */
  void deselect();
  /**
   * Enable visual flow for the Link.
   */
  void enableFlow();
  /**
   * Disable visual flow for the Link.
   */
  void disableFlow();
  /**
   *
   * @return true if visual flow is enabled
   */
  [[nodiscard]] bool isFlowEnabled() const;

  void setId(ax::NodeEditor::LinkId newId);

  ReadOnlyProperty<bool> hovered;
  ReadOnlyProperty<bool> selected;
  Event<> deleteEvent;
  Event<> doubleClickEvent;

 protected:
  void renderImpl() override;

  void setHovered(bool newHovered);

  ax::NodeEditor::LinkId id;

  Pin *inputPin;
  Pin *outputPin;

  Color color = Color::White;
  float thickness = 1.0f;

  bool valid = true;
  bool flowEnabled = false;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_LINK_H
