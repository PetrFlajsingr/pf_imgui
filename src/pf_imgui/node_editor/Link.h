//
// Created by xflajs00 on 27.03.2022.
//

#ifndef PF_IMGUI_NODE_EDITOR_LINK_H
#define PF_IMGUI_NODE_EDITOR_LINK_H

#include "fwd.h"
#include <imgui_node_editor.h>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Renderable.h>
#include <string>

namespace pf::ui::ig {

// TODO: comments
class Link : public Renderable {
  friend class NodeEditor;

 public:
  Link(const std::string &name, const ax::NodeEditor::LinkId &id, Pin *inputPin, Pin *outputPin);
  ~Link() override;

  [[nodiscard]] ax::NodeEditor::LinkId getId() const;
  [[nodiscard]] Pin &getInputPin() const;
  [[nodiscard]] Pin &getOutputPin() const;
  [[nodiscard]] bool isValid() const;
  void invalidate();

  [[nodiscard]] const ImVec4 &getColor() const;
  void setColor(const ImVec4 &newColor);
  [[nodiscard]] float getThickness() const;
  void setThickness(float newThickness);

  // change this in ItemElement too
  [[nodiscard]] PopupMenu &createOrGetPopupMenu();
  [[nodiscard]] bool hasPopupMenu() const;
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
