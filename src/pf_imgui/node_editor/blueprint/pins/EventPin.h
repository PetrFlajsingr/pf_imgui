//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_EVENTPIN_H
#define PF_IMGUI_BLUEPRINT_EVENTPIN_H

#include "Pin.h"
#include <imgui_internal.h>

namespace pf::ui::ig::bp {

class EventPin : public Pin {
 public:
  EventPin(const std::string &name, const std::string &label, Color color);

  [[nodiscard]] bool acceptsLinkWith(ig::Pin &other) const override;

 protected:
  void renderIcon() override;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_EVENTPIN_H
