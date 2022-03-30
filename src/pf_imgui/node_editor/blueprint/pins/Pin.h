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
  inline Pin(const std::string &name, const std::string &label, const ImColor &color)
      : pf::ui::ig::Pin(name, label), color(color) {}

  [[nodiscard]] inline const ImColor &getColor() const { return color; }
  inline void setColor(const ImColor &newColor) { color = newColor; }

 private:
  ImColor color;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_PIN_H
