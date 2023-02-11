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
#include <pf_imgui/details/LabeledSlider.h>
#include <pf_imgui/elements/details/SliderDetails.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/serialization.h>
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
class PF_IMGUI_EXPORT Slider : public ItemElement, public ValueContainer<T>, public Savable, public DragSource<T>, public DropTarget<T> {
 public:
  using MinMaxType = slider_details::MinMaxType<T>;
  constexpr static std::size_t ComponentCount = slider_details::getComponentCount<T>();
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
  Slider(std::string_view elementName, std::string_view labelText, MinMaxType minValue, MinMaxType maxValue, T initialValue = T{},
         Persistent persistent = Persistent::No, std::string numberFormat = slider_details::defaultFormat<MinMaxType>());

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

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::DragDropTarget, ColorOf::SliderGrab, ColorOf::SliderGrabActive,
               ColorOf::NavHighlight, ColorOf::Border, ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Property<Label> label;
  Property<T> value;

  [[nodiscard]] const T &getValue() const override;
  void setValue(const T &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const T &)> listener) override;

  void renderImpl() override;

  MinMaxType min;
  MinMaxType max;
  std::string format;
};

/**
 * @brief Slider with labels next to each component
 * @tparam T underlying value
 */
template<OneOf<PF_IMGUI_LABELEDSLIDER_TYPE_LIST> T>
class PF_IMGUI_EXPORT LabeledSlider : public Slider<T> {
 public:
  using MinMaxType = typename Slider<T>::MinMaxType;
  using Slider<T>::ComponentCount;
  /**
   * @brief Struct for construction of Slider.
   */
  struct Config {
    using Parent = LabeledSlider;
    Explicit<std::string_view> name;                                   /*!< Unique name of the element */
    Explicit<std::string_view> label;                                  /*!< Text rendered next to the element */
    Explicit<MinMaxType> min;                                          /*!< Minimum allowed value */
    Explicit<MinMaxType> max;                                          /*!< Maximum allowed value */
    T value{};                                                         /*!< Initial value */
    Explicit<std::array<std::string, ComponentCount>> componentLabels; /*!< Labels for components */
    Explicit<std::array<Color, ComponentCount>> componentLabelColors;  /*!< Colors for label for components */
    std::string format = slider_details::defaultFormat<MinMaxType>();  /*!< Format string for value rendering */
    bool persistent = false;                                           /*!< Allow state saving to disk */
  };
  /**
   * Construct Slider
   * @param config construction args @see Slider::Config
   */
  explicit LabeledSlider(Config &&config);
  /**
   * Construct Slider.
   * @param elementName ID of the slider
   * @param labelText text rendered next to the slider
   * @param minValue min value
   * @param maxValue max value
   * @param componentLabels labels for components
   * @param componentLabelColors colors for labels for components
   * @param initialValue starting value
   * @param persistent enable state saving to disk
   * @param format printf-like format for rendering value over slider
   */
  LabeledSlider(std::string_view elementName, std::string_view labelText, MinMaxType minValue, MinMaxType maxValue,
                std::array<std::string, ComponentCount> componentLabels, std::array<Color, ComponentCount> componentLabelColors,
                T initialValue = T{}, Persistent persistent = Persistent::No,
                std::string numberFormat = slider_details::defaultFormat<MinMaxType>());

  void setComponentLabels(std::array<std::string, ComponentCount> componentLabels);
  void setComponentLabelColors(std::array<Color, ComponentCount> componentColors);

 protected:
  void renderImpl() override;

 private:
  void loadCstrLabels() {
    for (std::size_t i = 0; i < ComponentCount; ++i) { labelsCstr[i] = labels[i].c_str(); }
  }

  std::array<std::string, ComponentCount> labels;
  std::array<const char *, ComponentCount> labelsCstr{nullptr};
  std::array<ImU32, ComponentCount> labelColors{};
};

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
Slider<T>::Slider(Slider::Config &&config)
    : Slider(config.name, config.label, config.min, config.max, config.value, config.persistent ? Persistent::Yes : Persistent::No,
             config.format) {}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
Slider<T>::Slider(std::string_view elementName, std::string_view labelText, Slider::MinMaxType minValue, Slider::MinMaxType maxValue,
                  T initialValue, Persistent persistent, std::string numberFormat)
    : ItemElement(elementName), Savable(persistent), DragSource<T>(false), DropTarget<T>(false), label(std::string{labelText}),
      value(initialValue), min(minValue), max(maxValue), format(std::move(numberFormat)) {}

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
  const auto address = &Prop_value(value);
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
    valueChanged =
        ImGui::SliderScalarN(label->get().c_str(), dataType, glm::value_ptr(*address), ComponentCount, &min, &max, format.c_str(), flags);
  }

  DragSource<T>::drag(*value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *value.modify() = *drop;
    return;
  }
  if (valueChanged) { Prop_triggerListeners(value); }
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

template<OneOf<PF_IMGUI_LABELEDSLIDER_TYPE_LIST> T>
LabeledSlider<T>::LabeledSlider(LabeledSlider::Config &&config)
    : LabeledSlider(config.name, config.label, config.min, config.max, config.componentLabels, config.componentLabelColors, config.value,
                    config.persistent ? Persistent::Yes : Persistent::No, config.format) {}

template<OneOf<PF_IMGUI_LABELEDSLIDER_TYPE_LIST> T>
LabeledSlider<T>::LabeledSlider(std::string_view elementName, std::string_view labelText, LabeledSlider::MinMaxType minValue,
                                LabeledSlider::MinMaxType maxValue, std::array<std::string, ComponentCount> componentLabels,
                                std::array<Color, ComponentCount> componentLabelColors, T initialValue, Persistent persistent,
                                std::string numberFormat)
    : Slider<T>(elementName, labelText, minValue, maxValue, initialValue, persistent, numberFormat), labels(componentLabels) {
  loadCstrLabels();
  setComponentLabelColors(componentLabelColors);
}

template<OneOf<PF_IMGUI_LABELEDSLIDER_TYPE_LIST> T>
void LabeledSlider<T>::setComponentLabelColors(std::array<Color, ComponentCount> componentColors) {
  for (std::size_t i = 0; i < ComponentCount; ++i) { labelColors[i] = static_cast<ImU32>(componentColors[i]); }
}

template<OneOf<PF_IMGUI_LABELEDSLIDER_TYPE_LIST> T>
void LabeledSlider<T>::setComponentLabels(std::array<std::string, ComponentCount> componentLabels) {
  labels = componentLabels;
  loadCstrLabels();
}

template<OneOf<PF_IMGUI_LABELEDSLIDER_TYPE_LIST> T>
void LabeledSlider<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = this->color.applyScoped();
  [[maybe_unused]] auto styleScoped = this->style.applyScoped();
  [[maybe_unused]] auto fontScoped = this->font.applyScopedIfNotDefault();
  auto valueChanged = false;
  const auto address = &PropertyOwner::Prop_value(this->value);
  const auto flags = ImGuiSliderFlags_AlwaysClamp;

  ImGuiDataType_ dataType;
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_FLOAT_TYPE_LIST>) {
    dataType = ImGuiDataType_Float;
  } else {
    dataType = ImGuiDataType_S32;
  }

  if constexpr (!OneOf<T, PF_IMGUI_SLIDER_GLM_TYPE_LIST>) {
    valueChanged = ImGui::LabeledSliderScalar(this->label->get().c_str(), dataType, address, &this->min, &this->max, this->format.c_str(),
                                              labelsCstr[0], labelColors[0], flags);
  } else {
    valueChanged = ImGui::LabeledSliderScalarN(this->label->get().c_str(), dataType, glm::value_ptr(*address), ComponentCount, &this->min,
                                               &this->max, this->format.c_str(), labelsCstr.data(), labelColors.data(), flags);
  }

  DragSource<T>::drag(*this->value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *this->value.modify() = *drop;
    return;
  }
  if (valueChanged) { PropertyOwner::Prop_triggerListeners(this->value); }
}

extern template class Slider<int>;
extern template class Slider<glm::ivec2>;
extern template class Slider<glm::ivec3>;
extern template class Slider<glm::ivec4>;
extern template class Slider<float>;
extern template class Slider<glm::vec2>;
extern template class Slider<glm::vec3>;
extern template class Slider<glm::vec4>;

extern template class LabeledSlider<int>;
extern template class LabeledSlider<glm::ivec2>;
extern template class LabeledSlider<glm::ivec3>;
extern template class LabeledSlider<glm::ivec4>;
extern template class LabeledSlider<float>;
extern template class LabeledSlider<glm::vec2>;
extern template class LabeledSlider<glm::vec3>;
extern template class LabeledSlider<glm::vec4>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SLIDER_H
