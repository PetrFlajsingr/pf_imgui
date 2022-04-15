//
// Created by xflajs00 on 30.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_NODEWITHLABEL_H
#define PF_IMGUI_BLUEPRINT_NODEWITHLABEL_H

#include <pf_imgui/node_editor/blueprint/nodes/Node.h>
#include <pf_imgui/interface/Labellable.h>

namespace pf::ui::ig::bp {

class NodeWithLabel : public Node, public Labellable {
 public:
  NodeWithLabel(const std::string &name, std::string label);

  toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 protected:
  void renderHeader() override;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_NODEWITHLABEL_H
