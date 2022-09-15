/**
 * @file Slider.h
 * @brief Slider element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_SLIDER_H
#define PF_IMGUI_ELEMENTS_SLIDER_H

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/details/SliderDetails.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/serialization.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

// TODO:
// ImGuiSliderFlags_Logarithmic
// ImGuiSliderFlags_NoRoundToFormat

/**
 * @brief Slider supporting multiple types.
 *
 * Supported types: int, float, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
 *
 * If the type is not a scalar, each scalar part of the structure gets its own slider.
 * @tparam T underlying type
 */
template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
class PF_IMGUI_EXPORT Slider : public ItemElement,
                               public ValueContainer<T>,
                               public Savable,
                               public DragSource<T>,
                               public DropTarget<T> {
 public:
  using MinMaxType = slider_details::MinMaxType<T>;
  /**
   * @brief Struct for construction of Slider.
   */
  struct Config {
    using Parent = Slider;
    Explicit<std::string_view> name;                                  /*!< Unique name of the element */
    Explicit<std::string_view> label;                                 /*!< Text rendered next to the element */
    Explicit<MinMaxType> min;                                         /*!< Minimum allowed value */
    Explicit<MinMaxType> max;                                         /*!< Maximum allowed value */
    T value{};                                                        /*!< Initial value */
    std::string format = slider_details::defaultFormat<MinMaxType>(); /*!< Format string for value rendering */
    bool persistent = false;                                          /*!< Allow state saving to disk */
  };
  /**
   * Construct Slider
   * @param config construction args @see Slider::Config
   */
  explicit Slider(Config &&config);
  /**
   * Construct Slider.
   * @param elementName ID of the slider
   * @param labelText text rendered next to the slider
   * @param minValue min value
   * @param maxValue max value
   * @param initialValue starting value
   * @param persistent enable state saving to disk
   * @param format printf-like format for rendering value over slider
   */
  Slider(std::string_view elementName, std::string_view labelText, MinMaxType minValue, MinMaxType maxValue,
         T initialValue = T{}, Persistent persistent = Persistent::No,
         std::string numberFormat = slider_details::defaultFormat<MinMaxType>());

  /**
   * Get min slider value.
   * @return min slider value
   */
  [[nodiscard]] MinMaxType getMin() const { return min; }
  /**
   * Set min slider value.
   * @param min new min slider value
   */
  void setMin(MinMaxType newMin) { min = newMin; }
  /**
   * Get max slider value.
   * @return max slider value
   */
  [[nodiscard]] MinMaxType getMax() const { return max; }
  /**
   * Set max slider value.
   * @param max new min slider value
   */
  void setMax(MinMaxType newMax) { max = newMax; }

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::FrameBackground,
               ColorOf::FrameBackgroundHovered, ColorOf::FrameBackgroundActive, ColorOf::DragDropTarget,
               ColorOf::SliderGrab, ColorOf::SliderGrabActive, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Observable<Label> label;
  ObservableProperty<Slider, T> value;

  [[nodiscard]] const T &getValue() const override;
  void setValue(const T &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const T &)> listener) override;

  void renderImpl() override;

 private:
  MinMaxType min;
  MinMaxType max;
  std::string format;
};

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
Slider<T>::Slider(Slider::Config &&config)
    : Slider(config.name, config.label, config.min, config.max, config.value,
             config.persistent ? Persistent::Yes : Persistent::No, config.format) {}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
Slider<T>::Slider(std::string_view elementName, std::string_view labelText, Slider::MinMaxType minValue,
                  Slider::MinMaxType maxValue, T initialValue, Persistent persistent, std::string numberFormat)
    : ItemElement(elementName), Savable(persistent), DragSource<T>(false), DropTarget<T>(false),
      label(std::string{labelText}), value(initialValue), min(minValue), max(maxValue),
      format(std::move(numberFormat)) {}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
toml::table Slider<T>::toToml() const {
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_GLM_TYPE_LIST>) {
    return toml::table{{"value", serializeGlmVec(*value)}};
  } else {
    return toml::table{{"value", *value}};
  }
}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
void Slider<T>::setFromToml(const toml::table &src) {
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_GLM_TYPE_LIST>) {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        const auto vecValue = safeDeserializeGlmVec<T>(*newVal);
        if (vecValue.has_value()) { *value.modify() = *vecValue; }
      }
    }
  } else {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.value<T>(); newVal.has_value()) { *value.modify() = *newVal; }
    }
  }
}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
void Slider<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto valueChanged = false;
  const auto address = &value.value;
  const auto flags = ImGuiSliderFlags_AlwaysClamp;

  ImGuiDataType_ dataType;
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_FLOAT_TYPE_LIST>) {
    dataType = ImGuiDataType_Float;
  } else {
    dataType = ImGuiDataType_S32;
  }

  if constexpr (!OneOf<T, PF_IMGUI_SLIDER_GLM_TYPE_LIST>) {
    valueChanged = ImGui::SliderScalar(label->get().c_str(), dataType, address, &min, &max, format.c_str(), flags);
  } else {
    valueChanged = ImGui::SliderScalarN(label->get().c_str(), dataType, glm::value_ptr(*address), T::length(), &min,
                                        &max, format.c_str(), flags);
  }

  DragSource<T>::drag(*value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *value.modify() = *drop;
    return;
  }
  if (valueChanged) { value.triggerListeners(); }
}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
const T &Slider<T>::getValue() const {
  return *value;
}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
Subscription Slider<T>::addValueListenerImpl(std::function<void(const T &)> listener) {
  return value.addListener(std::move(listener));
}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
void Slider<T>::setValue(const T &newValue) {
  *value.modify() = newValue;
}

extern template class Slider<int>;
extern template class Slider<glm::ivec2>;
extern template class Slider<glm::ivec3>;
extern template class Slider<glm::ivec4>;
extern template class Slider<float>;
extern template class Slider<glm::vec2>;
extern template class Slider<glm::vec3>;
extern template class Slider<glm::vec4>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SLIDER_H
