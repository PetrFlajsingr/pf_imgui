//
// Created by xflajs00 on 30.03.2022.
//

#ifndef PF_IMGUI_PIN_H
#define PF_IMGUI_PIN_H

#include <imgui.h>
#include <pf_imgui/node_editor/Pin.h>

namespace pf::ui::ig::bp {

class Pin : public pf::ui::ig::Pin {
 public:
  Pin(const std::string &name, const std::string &label, const ImColor &color);

  [[nodiscard]] const ImColor &getColor() const;
  void setColor(const ImColor &newColor);

  [[nodiscard]] bool acceptsLinkWith(pf::ui::ig::Pin &other) const override;

 protected:
  void addLink(std::shared_ptr<Link> link) override;

 private:
  ImColor color;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_PIN_H
