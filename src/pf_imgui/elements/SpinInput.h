/**
 * @file SpinInput.h
 * @brief Spinner element.
 * @author Petr Flajšingr
 * @date 23.5.21
 */

#ifndef PF_IMGUI_ELEMENTS_SPININPUT_H
#define PF_IMGUI_ELEMENTS_SPININPUT_H

#include <algorithm>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/details/Spin.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <pf_imgui/style/common.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Spinner for numeric types.
 * @tparam T inner type
 * @todo: format for float
 */
template<OneOf<int, float> T>
class PF_IMGUI_EXPORT SpinInput : public ItemElement,
                                  public ValueContainer<T>,
                                  public Savable,
                                  public DragSource<T>,
                                  public DropTarget<T> {
 public:
  /**
   * @brief Struct for construction of SpinInput.
   */
  struct Config {
    using Parent = SpinInput;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the element */
    Explicit<T> min;                  /*!< Minimum allowed value */
    Explicit<T> max;                  /*!< Maximum allowed value */
    T value{};                        /*!< Initial value */
    T step{1};                        /*!< Value change speed on interaction */
    T fastStep{100};                  /*!< Fast value change speed on interaction */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct SpinInput
   * @param config construction args @see SpinInput::Config
   */
  explicit SpinInput(Config &&config);
  /**
   * Construct SpinInput.
   * @param elementName ID of the element
   * @param labelText text rendered next to the element
   * @param initialValue starting value
   * @param valueStep spin step
   * @param valueStepFast fast spin step
   * @param persistent enable/disable state saving od disk
   */
  SpinInput(const std::string &elementName, const std::string &labelText, T minVal, T maxVal, T initialValue = T{},
            T valueStep = T{1}, const T &valueStepFast = T{100}, Persistent persistent = Persistent::No);

  [[nodiscard]] const T &getMin() const { return min; }
  void setMin(const T &minVal) { min = minVal; }

  [[nodiscard]] const T &getMax() const { return max; }
  void setMax(const T &maxVal) { max = maxVal; }

  [[nodiscard]] bool isReadOnly() const { return readOnly; }

  void setReadOnly(bool isReadOnly);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  void setValue(const T &newValue) override;
  [[nodiscard]] const T &getValue() const override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const T &)> listener) override;

 public:
  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::Button, ColorOf::ButtonHovered,
               ColorOf::ButtonActive, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::DragDropTarget, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow, ColorOf::TextSelectedBackground>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize, StyleOf::ButtonTextAlign> style;
  Font font = Font::Default();
  Observable<Label> label;
  ObservableProperty<SpinInput, T> value;

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
    : ItemElement(std::string{config.name.value}),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<T>(false), DropTarget<T>(false),
      label(std::string{config.label.value}), value(config.value), step(config.step), stepFast(config.fastStep),
      min(config.min), max(config.max) {}

template<OneOf<int, float> T>
SpinInput<T>::SpinInput(const std::string &elementName, const std::string &labelText, T minVal, T maxVal,
                        T initialValue, T valueStep, const T &valueStepFast, Persistent persistent)
    : ItemElement(elementName), Savable(persistent), DragSource<T>(false), DropTarget<T>(false), label(labelText),
      value(initialValue), step(valueStep), stepFast(valueStepFast), min(minVal), max(maxVal) {}

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
toml::table SpinInput<T>::toToml() const {
  return toml::table{{"value", *value}};
}

template<OneOf<int, float> T>
void SpinInput<T>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<T>(); newVal.has_value()) {
      *value.modify() = *newVal;
    }
  }
}

template<OneOf<int, float> T>
void SpinInput<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto valueChanged = false;
  if constexpr (std::same_as<T, int>) {
    valueChanged = ImGui::SpinInt(label->get().c_str(), &value.value, step, stepFast, flags);
  }
  if constexpr (std::same_as<T, float>) {
    valueChanged = ImGui::SpinFloat(label->get().c_str(), &value.value, step, stepFast, "%.3f",
                                    flags);  // TODO: user provided format
  }
  if (valueChanged) {
    value.value = std::clamp(value.value, min, max);
    value.triggerListeners();
  }
  DragSource<T>::drag(*value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *value.modify() = std::clamp(*drop, min, max);
  }
}
template<OneOf<int, float> T>
void SpinInput<T>::setValue(const T &newValue) {
  *value.modify() = newValue;
}

template<OneOf<int, float> T>
const T &SpinInput<T>::getValue() const {
  return *value;
}

template<OneOf<int, float> T>
Subscription SpinInput<T>::addValueListenerImpl(std::function<void(const T &)> listener) {
  return value.addListener(std::move(listener));
}

extern template class SpinInput<int>;
extern template class SpinInput<float>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SPININPUT_H
