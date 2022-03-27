//
// Created by xflajs00 on 27.03.2022.
//

#ifndef PF_IMGUI_NODE_EDITOR_NODE_H
#define PF_IMGUI_NODE_EDITOR_NODE_H

#include "Pin.h"
#include "fwd.h"
#include <imgui_node_editor.h>
#include <memory>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Renderable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

// TODO: comments
class Node : public Renderable {
  friend class NodeEditor;

 public:
  Node(const std::string &name);
  ~Node() override;

  [[nodiscard]] ax::NodeEditor::NodeId getId() const;
  [[nodiscard]] const std::vector<std::unique_ptr<Pin>> &getInputPins() const;
  [[nodiscard]] const std::vector<std::unique_ptr<Pin>> &getOutputPins() const;

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

  // change this in ItemElement too
  [[nodiscard]] PopupMenu &createOrGetPopupMenu();
  [[nodiscard]] bool hasPopupMenu() const;
  void removePopupMenu();

 protected:
  void renderImpl() override;

 private:
  int getNextPinId();

  ax::NodeEditor::NodeId id;
  NodeEditor *parent;

  std::vector<std::unique_ptr<Pin>> inputPins;
  std::vector<std::unique_ptr<Pin>> outputPins;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_NODE_H
