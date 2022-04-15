//
// Created by xflajs00 on 15.04.2022.
//

#ifndef PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H
#define PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H

#include <pf_imgui/interface/TomlSerializable.h>
#include <pf_imgui/node_editor/Node.h>
#include <static_type_info.h>

namespace pf::ui::ig::bp {

class Node : public ig::Node, public TomlSerializable {
 public:
  using ig::Node::Node;

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  [[nodiscard]] virtual static_type_info::TypeIndex getTypeId() const = 0;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H
