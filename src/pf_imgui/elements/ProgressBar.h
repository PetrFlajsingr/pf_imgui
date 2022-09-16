/**
 * @file ProgressBar.h
 * @brief ProgressBar element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_PROGRESSBAR_H
#define PF_IMGUI_ELEMENTS_PROGRESSBAR_H

#include <algorithm>
#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <string>

namespace pf::ui::ig {
/**
 * Required operations for type to be usable as ProgressBar value.
 * @tparam T
 */
template<typename T>
concept ProgressBarCompatible = requires(T t, float f) {
                                  { t + t } -> std::same_as<T>;
                                  { t *= f };
                                  { std::clamp(t, t, t) } -> std::convertible_to<T>;
                                } && std::convertible_to<float, T>;

/**
 * @brief Progress bar for notifying a user of operation progress.
 * @tparam T type storing the progress value
 */
template<ProgressBarCompatible T>
class PF_IMGUI_EXPORT ProgressBar : public ItemElement, public ValueContainer<T> {
 public:
  /**
   * Construct ProgressBar
   * @param config construction args @see ProgressBar::Config
   */
  struct Config {
    using Parent = ProgressBar;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Explicit<T> step;                /*!< Step taken when @code step() @endcode is called */
    Explicit<T> min;                 /*!< Lowest value representing 0% */
    Explicit<T> max;                 /*!< Highest value representing 100% */
    T value = min;                   /*!< Initial value within [min, max] */
    std::string overlay;             /*!< Text rendered on top of the element */
    Size size = Size::Auto();        /*!< Size of the element */
  };
  /**
   * Construct ProgressBar
   * @param config construction args @see ProgressBar::Config
   */
  explicit ProgressBar(Config &&config);
  /**
   * Construct ProgressBar.
   * @param elementName ID of the progress bar
   * @param valueStep step added to the current value when step() is called
   * @param minValue min value - used as a starting value when no value is provided
   * @param maxValue max value
   * @param initialValue starting value - min <= value <=max
   * @param overlayStr text rendered on top of the element
   * @param initialSize size of the progress bar
   */
  ProgressBar(std::string_view elementName, T valueStep, T minValue, T maxValue,
              std::optional<T> initialValue = std::nullopt, std::string overlayStr = "",
              Size initialSize = Size::Auto());

  /**
   * Set current percentage where min = 0% and max = 100%.
   * @param percentage <0.0f-1.0f>, gets clamped if outside this interval
   * @return current inner value of the progress bar
   */
  T setPercentage(float percentage);

  /**
   * Move inner value by step provided in constructor.
   * @return current inner value of the progress bar
   */
  T step();

  /**
   * Get min progress bar value.
   * @return min progress bar value
   */
  [[nodiscard]] T getMin() const { return min; }
  /**
   * Set min progress bar value.
   * @param newMin min progress bar value
   */
  void setMin(T newMin) { min = newMin; }
  /**
   * Get max progress bar value.
   * @return max progress bar value
   */
  [[nodiscard]] T getMax() const { return max; }
  /**
   * Set max progress bar value.
   * @param newMax min progress bar value
   */
  void setMax(T newMax) { max = newMax; }

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
  [[nodiscard]] float getCurrentPercentage() const;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::Border, ColorOf::BorderShadow, ColorOf::PlotHistogram>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();

  Property<Size> size;
  Property<T> value;

  [[nodiscard]] const T &getValue() const override;
  void setValue(const T &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const T &)> listener) override;

  void renderImpl() override;

 private:
  T stepValue;
  T min;
  T max;
  std::string overlay;
};

template<ProgressBarCompatible T>
ProgressBar<T>::ProgressBar(ProgressBar::Config &&config)
    : ProgressBar(config.name, config.step, config.min, config.max, config.value, config.overlay, config.size) {}

template<ProgressBarCompatible T>
ProgressBar<T>::ProgressBar(std::string_view elementName, T valueStep, T minValue, T maxValue,
                            std::optional<T> initialValue, std::string overlayStr, Size initialSize)
    : ItemElement(elementName), size(initialSize), value(initialValue.value_or(min)), stepValue(valueStep),
      min(minValue), max(maxValue), overlay(std::move(overlayStr)) {
  value.addListener([this](const auto &newValue) { *value.modify() = std::clamp(newValue, min, max); });
}

template<ProgressBarCompatible T>
T ProgressBar<T>::setPercentage(float percentage) {
  const auto newValue = min + (max - min) * percentage;
  *value.modify() = static_cast<T>(newValue);
  return *value;
}

template<ProgressBarCompatible T>
T ProgressBar<T>::step() {
  *value.modify() = *value + stepValue;
  return *value;
}

template<ProgressBarCompatible T>
float ProgressBar<T>::getCurrentPercentage() const {
  const auto diff = max - min;
  return (*value - min) / static_cast<float>(diff);
}

template<ProgressBarCompatible T>
void ProgressBar<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGui::ProgressBar(getCurrentPercentage(), static_cast<ImVec2>(*size), overlay.c_str());
}

template<ProgressBarCompatible T>
const T &ProgressBar<T>::getValue() const {
  return *value;
}

template<ProgressBarCompatible T>
void ProgressBar<T>::setValue(const T &newValue) {
  *value.modify() = newValue;
}

template<ProgressBarCompatible T>
Subscription ProgressBar<T>::addValueListenerImpl(std::function<void(const T &)> listener) {
  return value.addListener(std::move(listener));
}

extern template class ProgressBar<float>;
extern template class ProgressBar<int>;

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_PROGRESSBAR_H
