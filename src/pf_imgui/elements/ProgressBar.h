//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_PROGRESSBAR_H
#define PF_IMGUI_IMGUI_ELEMENTS_PROGRESSBAR_H

#include "interface/ItemElement.h"
#include "interface/ResizableElement.h"
#include "interface/ValueObservableElement.h"
#include <imgui.h>
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

template<typename T>
concept ProgressBarCompatible = requires(T t, float f) {
  {t + t};
  {t *= f};
  { std::clamp(t, t, t) }
  ->std::convertible_to<T>;
};

template<ProgressBarCompatible T>
class PF_IMGUI_EXPORT ProgressBar : public ItemElement, public ValueObservableElement<T>, public ResizableElement {
 public:
  ProgressBar(const std::string &elementName, T stepValue, T min, T max, std::optional<T> value = std::nullopt,
              const ImVec2 &size = {-1, 0})
      : Element(elementName),
        ItemElement(elementName), ValueObservableElement<T>(elementName, value.has_value() ? *value : min),
        ResizableElement(elementName, size), stepValue(stepValue), min(min), max(max) {}

  T setPercentage(float percentage) {
    percentage = std::clamp(percentage, 0.f, 1.f);
    const auto oldValue = ValueObservableElement<T>::getValue();
    const auto newValue = min + (max - min) * percentage;
    ValueObservableElement<T>::setValue(newValue);
    if (ValueObservableElement<T>::getValue() != oldValue) { ValueObservableElement<T>::notifyValueChanged(); }
    return newValue;
  }

  T step() {
    const auto oldValue = ValueObservableElement<T>::getValue();
    const auto newValue = std::clamp(oldValue + stepValue, min, max);
    ValueObservableElement<T>::setValue(newValue);
    if (ValueObservableElement<T>::getValue() != oldValue) { ValueObservableElement<T>::notifyValueChanged(); }
    return newValue;
  }

  float getCurrentPercentage() {
    const auto size = max - min;
    return (ValueObservableElement<T>::getValue() - min) / static_cast<float>(size);
  }

 protected:
  void renderImpl() override { ImGui::ProgressBar(getCurrentPercentage(), getSize()); }

 private:
  T stepValue;
  T min;
  T max;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_PROGRESSBAR_H
