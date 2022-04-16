//
// Created by xflajs00 on 30.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_NODEWITHLABEL_H
#define PF_IMGUI_BLUEPRINT_NODEWITHLABEL_H

#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/node_editor/blueprint/common.h>
#include <pf_imgui/node_editor/blueprint/nodes/Node.h>

namespace pf::ui::ig::bp {

class NodeWithLabel : public Node, public Labellable {
 public:
  PF_IMGUI_BLUEPRINT_NODE_ID(NodeWithLabel)
  NodeWithLabel(const std::string &name, std::string label);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  [[nodiscard]] static std::unique_ptr<NodeWithLabel> ConstructFromToml(ig::NodeEditor *parent, const toml::table &src);

 protected:
  void renderHeader() override;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_NODEWITHLABEL_H
