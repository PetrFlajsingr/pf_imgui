//
// Created by xflajs00 on 30.03.2022.
//

#ifndef PF_IMGUI_NODEWITHHEADER_H
#define PF_IMGUI_NODEWITHHEADER_H

#include <pf_imgui/node_editor/Node.h>

namespace pf::ui::ig::bp {

class NodeWithHeader : public Node {
 public:
  NodeWithHeader(const std::string &name, std::string label);

 protected:
  void renderHeader() override;

 private:
  std::string label;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_NODEWITHHEADER_H
