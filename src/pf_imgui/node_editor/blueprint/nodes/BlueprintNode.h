//
// Created by xflajs00 on 15.04.2022.
//

#ifndef PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H
#define PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H

#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/node_editor/Node.h>

namespace pf::ui::ig::bp {

class BlueprintNode : public Node, public Savable {
 public:
  inline BlueprintNode(const std::string &name, Persistent persistent) : Node(name), Savable(persistent) {}
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMUI_NODE_EDITOR_BLUEPRINTNODE_H
