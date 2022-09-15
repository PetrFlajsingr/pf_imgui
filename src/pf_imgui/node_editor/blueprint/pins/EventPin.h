//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_EVENTPIN_H
#define PF_IMGUI_BLUEPRINT_EVENTPIN_H

#include "Pin.h"
#include <imgui_internal.h>
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {

class EventPin : public Pin {
 public:
  PF_IMGUI_BLUEPRINT_PIN_ID(EventPin)
  EventPin(std::string_view elementName, std::string_view labelText, Color pinColor);

  [[nodiscard]] bool acceptsLinkWith(ig::Pin &other) const override;

  [[nodiscard]] static std::unique_ptr<EventPin> ConstructFromToml(ig::Node *parent, const toml::table &src);

 protected:
  void renderIcon() override;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_EVENTPIN_H
