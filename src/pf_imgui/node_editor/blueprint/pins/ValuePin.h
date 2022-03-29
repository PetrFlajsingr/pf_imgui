//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_VALUEPIN_H
#define IMGUI_EXPERIMENTS_VALUEPIN_H

#include "PinDraw.h"
#include <pf_imgui/node_editor/Pin.h>
#include <string>

namespace pf::ui::ig::bp {

class ValuePin : public Pin {
 public:
  ValuePin(const std::string &name, const std::string &label, const ImColor &color);

  [[nodiscard]] const ImColor &getColor() const;
  void setColor(const ImColor &newColor);

 protected:
  void renderIcon() override;

 private:
  ImColor color;
};

}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_VALUEPIN_H
