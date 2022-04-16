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

  [[nodiscard]] static std::unique_ptr<DragPin> ConstructFromToml(const toml::table &src) {
    auto result = std::make_unique<DragPin>("", "", Color::White);
    result->setFromToml(src);
    return std::move(result);
  }

  [[nodiscard]] toml::table toToml() const override {
    toml::table result = InteractablePin<DragInput<T>>::toToml();
    if (auto data = result.find("data"); data != result.end()) {
      data->second.as_table()->insert_or_assign("speed", InteractablePin<DragInput<T>>::inputElement->getSpeed());
      data->second.as_table()->insert_or_assign("min", InteractablePin<DragInput<T>>::inputElement->getMin());
      data->second.as_table()->insert_or_assign("max", InteractablePin<DragInput<T>>::inputElement->getMax());
      data->second.as_table()->insert_or_assign("format", InteractablePin<DragInput<T>>::inputElement->getFormat());
    }
    return result;
  }
  void setFromToml(const toml::table &src) override {
    InteractablePin<DragInput<T>>::setFromToml(src);
    if (auto data = src.find("data"); data != src.end()) {
      if (auto dataTable = data->second.as_table(); dataTable != nullptr) {
        if (auto speed = dataTable->find("speed"); speed != dataTable->end()) {
          if (auto speedValue = speed->second.as_floating_point(); speedValue != nullptr) {
            InteractablePin<DragInput<T>>::inputElement->setSpeed(speedValue->get());
          }
        }
        if (auto min = dataTable->find("min"); min != dataTable->end()) {
          if (auto minValue = min->second.as_floating_point(); minValue != nullptr) {
            InteractablePin<DragInput<T>>::inputElement->setMin(minValue->get());
          }
        }
        if (auto max = dataTable->find("max"); max != dataTable->end()) {
          if (auto maxValue = max->second.as_floating_point(); maxValue != nullptr) {
            InteractablePin<DragInput<T>>::inputElement->setMax(maxValue->get());
          }
        }
        if (auto format = dataTable->find("format"); format != dataTable->end()) {
          if (auto formatValue = format->second.as_string(); formatValue != nullptr) {
            InteractablePin<DragInput<T>>::inputElement->setFormat(formatValue->get());
          }
        }
      }
    }
  }

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
