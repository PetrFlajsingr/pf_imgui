//
// Created by xflajs00 on 30.03.2022.
//

#ifndef PF_IMGUI_SIMPLENODE_H
#define PF_IMGUI_SIMPLENODE_H

#include <pf_imgui/node_editor/Node.h>

namespace pf::ui::ig::bp {

class SimpleNode : public Node {
 public:
  SimpleNode(const std::string &name);

 protected:
  virtual void renderIcon() = 0;

  void renderMiddle() override;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_SIMPLENODE_H
