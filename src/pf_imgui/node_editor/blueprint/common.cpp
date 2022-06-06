//
// Created by xflajs00 on 16.04.2022.
//

#include "common.h"
#include <pf_imgui/node_editor/blueprint/nodes/Node.h>
#include <pf_imgui/node_editor/blueprint/pins/Pin.h>

namespace pf::ui::ig::bp {

std::optional<std::unique_ptr<Pin>> NodeEditorLoading::constructPin(PinTypeIdentifier typeId, ig::Node *parent, //-V813
                                                                    const toml::table &src) {
  if (auto iter = pinConstructors.find(typeId); iter != pinConstructors.end()) { return iter->second(parent, src); }
  return std::nullopt;
}
std::optional<std::unique_ptr<Node>> NodeEditorLoading::constructNode(NodeTypeIdentifier typeId, ig::NodeEditor *parent, //-V813
                                                                      const toml::table &src) {
  if (auto iter = nodeConstructors.find(typeId); iter != nodeConstructors.end()) { return iter->second(parent, src); }
  return std::nullopt;
}
}  // namespace pf::ui::ig::bp
