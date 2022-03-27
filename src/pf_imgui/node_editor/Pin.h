//
// Created by xflajs00 on 27.03.2022.
//

#ifndef PF_IMGUI_NODE_EDITOR_PIN_H
#define PF_IMGUI_NODE_EDITOR_PIN_H

#include "fwd.h"
#include <imgui_node_editor.h>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {

// TODO: customization
// TODO: link validation
// TODO: code link
// TODO: comments
class Pin : public Renderable, public Labellable {
  friend class NodeEditor;
  friend class Node;

 public:
  enum class Type {
    Input = static_cast<int>(ax::NodeEditor::PinKind::Input),
    Output = static_cast<int>(ax::NodeEditor::PinKind::Output)
  };

  Pin(const std::string &name, const std::string &label);
  ~Pin() override;

  [[nodiscard]] ax::NodeEditor::PinId getId() const;

  [[nodiscard]] Type getType() const;

  [[nodiscard]] const std::vector<std::shared_ptr<Link>> &getLinks() const;

  [[nodiscard]] Node &getNode();
  [[nodiscard]] const Node &getNode() const;

  Subscription addLinkListener(std::invocable<std::shared_ptr<Link>> auto &&listener) {
    return observableLink.addListener(std::forward<decltype(listener)>(listener));
  }

  [[nodiscard]] bool hasAnyValidLinks() const;

  [[nodiscard]] const ImVec4 &getValidLinkPreviewColor() const;
  void setValidLinkPreviewColor(const ImVec4 &color);

  [[nodiscard]] float getValidLinkPreviewThickness() const;
  void setValidLinkPreviewThickness(float thickness);

  [[nodiscard]] const ImVec4 &getInvalidLinkPreviewColor() const;
  void setInvalidLinkPreviewColor(const ImVec4 &color);

  [[nodiscard]] float getInvalidLinkPreviewThickness() const;
  void setInvalidLinkPreviewThickness(float thickness);

  [[nodiscard]] const ImVec4 &getUnconnectedLinkPreviewColor() const;
  void setUnconnectedLinkPreviewColor(const ImVec4 &color);

  [[nodiscard]] float getUnconnectedLinkPreviewThickness() const;
  void setUnconnectedLinkPreviewThickness(float thickness);

  // change this in ItemElement too
  [[nodiscard]] PopupMenu &createOrGetPopupMenu();
  [[nodiscard]] bool hasPopupMenu() const;
  void removePopupMenu();

 protected:
  constexpr static int LINK_CLEANUP_FREQUENCY = 100;
  int sinceLastLinkCleanup = LINK_CLEANUP_FREQUENCY;

  /**
   * @warning Do not override unless you completely want to change the way pins look
   */
  void renderImpl() override;

  virtual void renderIcon();

 private:
  ax::NodeEditor::PinId id;
  Type type;

  Node *parent;

  ImVec4 validLinkPreviewColor = ImVec4(1, 1, 1, 1);
  float validLinkPreviewThickness = 1.f;

  ImVec4 invalidLinkPreviewColor = ImVec4(1, 1, 1, 1);
  float invalidLinkPreviewThickness = 1.f;

  ImVec4 unconnectedLinkPreviewColor = ImVec4(1, 1, 1, 1);
  float unconnectedLinkPreviewThickness = 1.f;

  std::vector<std::shared_ptr<Link>> links;
  Observable_impl<std::shared_ptr<Link>> observableLink;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_PIN_H
