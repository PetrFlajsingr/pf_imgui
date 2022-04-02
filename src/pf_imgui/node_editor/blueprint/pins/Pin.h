//
// Created by xflajs00 on 30.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_PIN_H
#define PF_IMGUI_BLUEPRINT_PIN_H

#include <imgui.h>
#include <pf_imgui/node_editor/Pin.h>

namespace pf::ui::ig::bp {

class Pin : public ig::Pin {
 public:
  Pin(const std::string &name, const std::string &label, Color color);

  [[nodiscard]] Color getColor() const;
  void setColor(Color newColor);

  [[nodiscard]] bool acceptsLinkWith(ig::Pin &other) const override;

 protected:
  void addLink(Link &link) override;

 private:
  Color color;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_PIN_H
