//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_PROGRESSBAR_H
#define PF_IMGUI_IMGUI_ELEMENTS_PROGRESSBAR_H

#include "interface/ItemElement.h"
#include "interface/Resizable.h"
#include "interface/ValueObservable.h"
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
class PF_IMGUI_EXPORT ProgressBar : public ItemElement, public ValueObservable<T>, public Resizable {
 public:
  ProgressBar(const std::string &elementName, T stepValue, T min, T max, std::optional<T> value = std::nullopt,
              const ImVec2 &size = {-1, 0})
      : Element(elementName),
        ItemElement(elementName), ValueObservable<T>(elementName, value.has_value() ? *value : min), Resizable(size),
        stepValue(stepValue), min(min), max(max) {}

  T setPercentage(float percentage) {
    percentage = std::clamp(percentage, 0.f, 1.f);
    const auto oldValue = ValueObservable<T>::getValue();
    const auto newValue = min + (max - min) * percentage;
    ValueObservable<T>::setValue(newValue);
    if (ValueObservable<T>::getValue() != oldValue) { ValueObservable<T>::notifyValueChanged(); }
    return newValue;
  }

  T step() {
    const auto oldValue = ValueObservable<T>::getValue();
    const auto newValue = std::clamp(oldValue + stepValue, min, max);
    ValueObservable<T>::setValue(newValue);
    if (ValueObservable<T>::getValue() != oldValue) { ValueObservable<T>::notifyValueChanged(); }
    return newValue;
  }

  float getCurrentPercentage() {
    const auto size = max - min;
    return (ValueObservable<T>::getValue() - min) / static_cast<float>(size);
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
