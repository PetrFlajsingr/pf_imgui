//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_EVENTPIN_H
#define IMGUI_EXPERIMENTS_EVENTPIN_H

#include "ValuePin.h"
#include <imgui_internal.h>

namespace pf::ui::ig::bp {

class EventPin : public ValuePin {
 public:
  EventPin(const std::string &name, const std::string &label, const ImColor &color);

 protected:
  void renderIcon() override;
};

}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_EVENTPIN_H
