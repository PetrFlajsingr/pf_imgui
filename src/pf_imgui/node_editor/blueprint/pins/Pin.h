//
// Created by xflajs00 on 30.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_PIN_H
#define PF_IMGUI_BLUEPRINT_PIN_H

#include <imgui.h>
#include <pf_imgui/interface/TomlSerializable.h>
#include <pf_imgui/node_editor/Pin.h>
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {
class Node;
class NodeEditor;
class Pin : public ig::Pin, public TomlSerializable {
  friend class Node;
  friend class NodeEditor;

 public:
  Pin(std::string_view elementName, std::string_view labelText, Color pinColor);

  [[nodiscard]] Color getColor() const;
  void setColor(Color newColor);

  [[nodiscard]] bool acceptsLinkWith(ig::Pin &other) const override;

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  [[nodiscard]] virtual PinTypeIdentifier getPinTypeId() const = 0;

 protected:
  void addLink(Link &link) override;

 private:
  Color color;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_PIN_H
