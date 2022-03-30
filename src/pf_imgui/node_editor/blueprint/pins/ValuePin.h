//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_VALUEPIN_H
#define IMGUI_EXPERIMENTS_VALUEPIN_H

#include "Pin.h"
#include "PinDraw.h"
#include <string>

namespace pf::ui::ig::bp {

class ValuePin : public Pin {
 public:
  ValuePin(const std::string &name, const std::string &label, const ImColor &color);

 protected:
  void renderIcon() override;
};

}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_VALUEPIN_H
