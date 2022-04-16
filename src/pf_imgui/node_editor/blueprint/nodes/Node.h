//
// Created by xflajs00 on 15.04.2022.
//

#ifndef PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H
#define PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H

#include <pf_imgui/interface/TomlSerializable.h>
#include <pf_imgui/node_editor/Node.h>
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {
class NodeEditor;
class Node : public ig::Node, public TomlSerializable {
  friend class NodeEditor;

 public:
  using ig::Node::Node;

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  [[nodiscard]] virtual NodeTypeIdentifier getNodeTypeId() const = 0;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H
