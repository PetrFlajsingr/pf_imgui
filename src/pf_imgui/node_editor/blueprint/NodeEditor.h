//
// Created by xflajs00 on 16.04.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_NODEEDITOR_H
#define PF_IMGUI_BLUEPRINT_NODEEDITOR_H

#include <pf_imgui/interface/TomlSerializable.h>
#include <pf_imgui/node_editor/NodeEditor.h>

namespace pf::ui::ig::bp {

class NodeEditor : public ig::NodeEditor, public TomlSerializable {
 public:
  /**
   * @brief Struct for construction of NodeEditor.
   */
  struct Config {
    using Parent = NodeEditor;
    std::string name;         /*!< Unique name of the element */
    Size size = Size::Auto(); /*!< Size of the element */
  };
  NodeEditor(const std::string &name, const Size &size);
  explicit NodeEditor(Config &&config);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 private:
  [[nodiscard]] toml::array linksToToml() const;
  [[nodiscard]] toml::array commentsToToml() const;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_NODEEDITOR_H
