//
// Created by xflajs00 on 30.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_SIMPLENODE_H
#define PF_IMGUI_BLUEPRINT_SIMPLENODE_H

#include <pf_imgui/node_editor/blueprint/nodes/Node.h>

namespace pf::ui::ig::bp {

class SimpleNode : public Node {
 public:
  explicit SimpleNode(const std::string &elementName);

 protected:
  virtual void renderIcon() = 0;

  void renderMiddle() override;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_SIMPLENODE_H
