//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_ARRAYPIN_H
#define IMGUI_EXPERIMENTS_ARRAYPIN_H

#include "ValuePin.h"
#include <imgui_internal.h>

namespace pf::ui::ig::bp {

class ArrayPin : public ValuePin {
 public:
  ArrayPin(const std::string &name, const std::string &label, const ImVec4 &color);

 protected:
  void renderIcon() override;
};

}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_ARRAYPIN_H
