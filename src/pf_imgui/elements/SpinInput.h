/**
 * @file SpinInput.h
 * @brief Spinner element.
 * @author Petr Flajšingr
 * @date 23.5.21
 */

#ifndef PF_IMGUI_ELEMENTS_SPININPUT_H
#define PF_IMGUI_ELEMENTS_SPININPUT_H

#include <algorithm>
#include <pf_imgui/_export.h>
#include <pf_imgui/details/Spin.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Spinner for numeric types.
 * @tparam T inner type
 * @todo: format for float
 */
template<OneOf<int, float> T>
class PF_IMGUI_EXPORT SpinInput
    : public ItemElement,
      public Labellable,
      public ValueObservable<T>,
      public Savable,
      public DragSource<T>,
      public DropTarget<T>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::DragDropTarget,
                               style::ColorOf::Button, style::ColorOf::ButtonHovered, style::ColorOf::ButtonActive,
                               style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::DragDropTarget,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow,
                               style::ColorOf::TextSelectedBackground>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize,
                               style::Style::ButtonTextAlign> {
 public:
  /**
   * @brief Struct for construction of SpinInput.
   */
  struct Config {
    using Parent = SpinInput;
    std::string_view name;   /*!< Unique name of the element */
    std::string_view label;  /*!< Text rendered next to the element */
    T min;                   /*!< Minimum allowed value */
    T max;                   /*!< Maximum allowed value */
    T value{};               /*!< Initial value */
    T step{1};               /*!< Value change speed on interaction */
    T fastStep{100};         /*!< Fast value change speed on interaction */
    bool persistent = false; /*!< Allow state saving to disk */
  };
  /**
   * Construct SpinInput
   * @param config construction args @see SpinInput::Config
   */
  explicit SpinInput(Config &&config);
  /**
   * Construct SpinInput.
   * @param elementName ID of the element
   * @param label texte rendered next to the element
   * @param value starting value
   * @param step spin step
   * @param stepFast fast spin step
   * @param persistent enable/disable state saving od disk
   */
  SpinInput(const std::string &elementName, const std::string &label, T minVal, T maxVal, T value = T{}, T step = T{1},
            const T &stepFast = T{100}, Persistent persistent = Persistent::No);

  [[nodiscard]] const T &getMin() const { return min; }
  void setMin(const T &minVal) { min = minVal; }

  [[nodiscard]] const T &getMax() const { return max; }
  void setMax(const T &maxVal) { max = maxVal; }

  [[nodiscard]] bool isReadOnly() const { return readOnly; }

  void setReadOnly(bool isReadOnly);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 protected:
  void renderImpl() override;

 private:
  T step;
  T stepFast;
  T min;
  T max;

  bool readOnly = false;
  ImGuiInputTextFlags flags = {};
};

template<OneOf<int, float> T>
SpinInput<T>::SpinInput(SpinInput::Config &&config)
    : ItemElement(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable<T>(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<T>(false), DropTarget<T>(false),
      step(config.step), stepFast(config.fastStep), min(config.min), max(config.max) {}

template<OneOf<int, float> T>
SpinInput<T>::SpinInput(const std::string &elementName, const std::string &label, T minVal, T maxVal, T value, T step,
                        const T &stepFast, Persistent persistent)
    : ItemElement(elementName), Labellable(label), ValueObservable<T>(value),
      Savable(persistent), DragSource<T>(false), DropTarget<T>(false), step(step), stepFast(stepFast), min(minVal),
      max(maxVal) {}

template<OneOf<int, float> T>
void SpinInput<T>::setReadOnly(bool isReadOnly) {
  readOnly = isReadOnly;
  if (readOnly) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  } else {
    flags &= ~ImGuiInputTextFlags_ReadOnly;
  }
}

template<OneOf<int, float> T>
toml::table SpinInput<T>::toToml() const { return toml::table{{"value", ValueObservable<T>::getValue()}}; }

template<OneOf<int, float> T>
void SpinInput<T>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<T>(); newVal.has_value()) {
      ValueObservable<T>::setValueAndNotifyIfChanged(*newVal);
    }
  }
}

template<OneOf<int, float> T>
void SpinInput<T>::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  auto valueChanged = false;
  if constexpr (std::same_as<T, int>) {
    valueChanged = ImGui::SpinInt(getLabel().c_str(), ValueObservable<T>::getValueAddress(), step, stepFast, flags);
  }
  if constexpr (std::same_as<T, float>) {
    valueChanged = ImGui::SpinFloat(getLabel().c_str(), ValueObservable<T>::getValueAddress(), step, stepFast, "%.3f",
                                    flags);  // TODO: user provided format
  }
  if (valueChanged) {
    ValueObservable<T>::setValueInner(std::clamp(ValueObservable<T>::getValue(), min, max));
    ValueObservable<T>::notifyValueChanged();
  }
  DragSource<T>::drag(ValueObservable<T>::getValue());
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    const auto value = std::clamp(*drop, min, max);
    ValueObservable<T>::setValueAndNotifyIfChanged(value);
    return;
  }
}

extern template class SpinInput<int>;
extern template class SpinInput<float>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SPININPUT_H
