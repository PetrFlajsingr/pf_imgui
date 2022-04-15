//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_DRAGPIN_H
#define PF_IMGUI_BLUEPRINT_DRAGPIN_H

#include "InteractablePin.h"
#include <pf_imgui/elements/DragInput.h>
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {

// TODO: min max speed setters
template<OneOf<IMGUI_DRAG_TYPE_LIST> T>
class DragPin : public InteractablePin<DragInput<T>> {
 public:
  PF_IMGUI_BLUEPRINT_OVERRIDE_GETTYPEID(DragPin)
  struct DragConfig {
    Width width;
    typename DragInput<T>::ParamType speed;
    typename DragInput<T>::ParamType min;
    typename DragInput<T>::ParamType max;
    T value{};
    std::string format = ui::ig::details::defaultDragFormat<T>();
  };

  DragPin(const std::string &name, const std::string &label, Color color, DragConfig &&config)
      : InteractablePin<DragInput<T>>(name, label, color, config.width,
                                      CreateDragConfig(uniqueId(), label, std::move(config))) {}

 protected:
  static typename DragInput<T>::Config CreateDragConfig(const std::string &name, const std::string &label,
                                                        DragConfig &&config) {
    return {.name = name,
            .label = label,
            .speed = config.speed,
            .min = config.min,
            .max = config.max,
            .value = config.value,
            .format = std::move(config.format)};
  }
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_DRAGPIN_H
