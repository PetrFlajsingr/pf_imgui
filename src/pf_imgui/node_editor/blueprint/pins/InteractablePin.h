//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_INTERACTABLEPIN_H
#define PF_IMGUI_BLUEPRINT_INTERACTABLEPIN_H

#include "PinWithValue.h"
#include <concepts>
#include <imgui_internal.h>
#include <pf_common/specializations.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/interface/decorators/WidthDecorator.h>

namespace pf::ui::ig::bp {

template<std::derived_from<Renderable> T>
  requires derived_specialization_of<T, ValueObservable>
class InteractablePin : public PinWithValue<typename T::ValueType> {
 public:
  using ValueType = typename PinWithValue<typename T::ValueType>::ValueType;

  template<typename... Args>
  InteractablePin(const std::string &name, const std::string &label, Color color, Width width, Args &&...args)
      : PinWithValue<ValueType>(name, label, color),
        inputElement(std::make_unique<WidthDecorator<T>>(width, std::forward<Args>(args)...)) {}

  [[nodiscard]] ValueType getValue() const override { return inputElement->getValue(); }
  void setValue(const ValueType &value) override { inputElement->setValue(value); }

  Subscription addValueListener(std::invocable<ValueType> auto &&listener) {
    return inputElement->addValueListener(std::forward<decltype(listener)>(listener));
  }

  virtual [[nodiscard]] toml::table toToml() const override {
    auto result = Pin::toToml();
    if constexpr (std::derived_from<T, TomlSerializable>) {
      auto elementData = inputElement->toToml();
      elementData.insert_or_assign("width", static_cast<float>(inputElement->getWidth()));
      result.insert_or_assign("data", elementData);
    }
    return result;
  }

  virtual void setFromToml(const toml::table &src) override {
    Pin::setFromToml(src);
    if constexpr (std::derived_from<T, TomlSerializable>) {
      if (auto data = src.find("data"); data != src.end()) {
        if (auto dataTable = data->second.as_table(); dataTable != nullptr) {
          inputElement->setFromToml(*dataTable);
          if (auto width = dataTable->find("width"); width != dataTable->end()) {
            if (auto widthVal = width->second.as_floating_point(); widthVal != nullptr) {
              inputElement->setWidth(static_cast<float>(widthVal->get()));
            }
          }
        }
      }
    }
  }

 protected:
  void renderInfo() override {
    if (PinWithValue<ValueType>::getType() == Pin::Type::Input && !PinWithValue<ValueType>::hasAnyValidLinks()) {
      inputElement->render();
    } else {
      ValuePin<ValueType>::renderInfo();
    }
  }

  std::unique_ptr<WidthDecorator<T>> inputElement;
};
}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_INTERACTABLEPIN_H
