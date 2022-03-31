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
#include <string>

namespace pf::ui::ig {

/**
 * @brief Link connecting two Pins in NodeEditor.
 */
class Link : public Renderable {
  friend class NodeEditor;

 public:
  /**
   * Construct Link
   * @param name unique name of the element
   * @param id unique internal id
   * @param inputPin input pin
   * @param outputPin output pin
   */
  Link(const std::string &name, const ax::NodeEditor::LinkId &id, Pin *inputPin, Pin *outputPin);
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
  [[nodiscard]] const ImVec4 &getColor() const;
  /**
   * Set color of the Link line
   * @param newColor new color
   */
  void setColor(const ImVec4 &newColor);
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

 protected:
  void renderImpl() override;

 private:
  ax::NodeEditor::LinkId id;

  Pin *inputPin;
  Pin *outputPin;

  ImVec4 color = ImVec4(1, 1, 1, 1);
  float thickness = 1.0f;

  bool valid = true;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_LINK_H
