/**
 * @file ProgressBar.h
 * @brief ProgressBar element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_PROGRESSBAR_H
#define PF_IMGUI_ELEMENTS_PROGRESSBAR_H

#include <imgui.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

/**
 * Required operations for type to be usable as ProgressBar value.
 * @tparam T
 */
template<typename T>
concept ProgressBarCompatible = requires(T t, float f) {
  {t + t};
  {t *= f};
  { std::clamp(t, t, t) } -> std::convertible_to<T>;
};

/**
 * @brief Progress bar for notifying a user of operation progress.
 * @tparam T type storing the progress value
 */
template<ProgressBarCompatible T>
class PF_IMGUI_EXPORT ProgressBar : public ItemElement, public ValueObservable<T>, public Resizable {
 public:
  /**
   * Construct ProgressBar.
   * @param elementName ID of the progress bar
   * @param stepValue step added to the current value when step() is called
   * @param min min value - used as a starting value when no value is provided
   * @param max max value
   * @param value starting value - min <= value <=max
   * @param size size of the progress bar
   */
  ProgressBar(const std::string &elementName, T stepValue, T min, T max, std::optional<T> value = std::nullopt,
              const ImVec2 &size = {-1, 0})
      : Element(elementName),
        ItemElement(elementName), ValueObservable<T>(elementName, value.has_value() ? *value : min), Resizable(size),
        stepValue(stepValue), min(min), max(max) {}

  /**
   * Set current percentage where min = 0% and max = 100%.
   * @param percentage <0.0f-1.0f>, gets clamped if outside this interval
   * @return current inner value of the progress bar
   */
  T setPercentage(float percentage) {
    percentage = std::clamp(percentage, 0.f, 1.f);
    const auto oldValue = ValueObservable<T>::getValue();
    const auto newValue = min + (max - min) * percentage;
    ValueObservable<T>::setValueInner(newValue);
    if (ValueObservable<T>::getValue() != oldValue) { ValueObservable<T>::notifyValueChanged(); }
    return newValue;
  }

  /**
   * Move inner value by step provided in constructor.
   * @return current inner value of the progress bar
   */
  T step() {
    const auto oldValue = ValueObservable<T>::getValue();
    const auto newValue = std::clamp(oldValue + stepValue, min, max);
    ValueObservable<T>::setValueInner(newValue);
    if (ValueObservable<T>::getValue() != oldValue) { ValueObservable<T>::notifyValueChanged(); }
    return newValue;
  }

  /**
   * Get min progress bar value.
   * @return min progress bar value
   */
  [[nodiscard]] T getMin() const { return min; }
  /**
   * Set min progress bar value.
   * @param min min progress bar value
   */
  void setMin(T min) { ProgressBar::min = min; }
  /**
   * Get max progress bar value.
   * @return max progress bar value
   */
  [[nodiscard]] T getMax() const { return max; }
  /**
   * Set max progress bar value.
   * @param max min progress bar value
   */
  void setMax(T max) { ProgressBar::max = max; }

  /**
   * Get step value which is used in step().
   * @return step value
   */
  [[nodiscard]] T getStep() const { return stepValue; }
  /**
   * Set new step value used for step().
   * @param step new step value
   */
  void setStep(T step) { stepValue = step; }

  /**
   * Get current percentage of the progress bar <0.0f-1.0f>.
   * @return current percentage of the progress bar <0.0f-1.0f>
   */
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
#endif//PF_IMGUI_ELEMENTS_PROGRESSBAR_H
