/**
 * @file DragInput.h
 * @brief DragInput element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_DRAGINPUT_H
#define PF_IMGUI_ELEMENTS_DRAGINPUT_H

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/math/Range.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/details/LabeledDrag.h>
#include <pf_imgui/elements/details/DragInputDetails.h>
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

// TODO: support for more data types
/**
 * @brief Numeric input supporting either keyboard input or mouse drag.
 *
 * Type of the DragInput is based on underlying types. There is a separate input for each scalar part of type T.
 *
 * Supported types: int, float, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4, math::Range<int>, math::Range<float>
 *
 * @tparam T Underlying type
 */
template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
class PF_IMGUI_EXPORT DragInput : public ItemElement, public ValueContainer<T>, public Savable, public DragSource<T>, public DropTarget<T> {
 public:
  using ParamType = drag_details::UnderlyingType<T>;
  constexpr static std::size_t ComponentCount = drag_details::getComponentCount<T>();

  /**
   * @brief Struct for construction of DragInput.
   */
  struct Config {
    using Parent = DragInput;
    Explicit<std::string_view> name;                       /*!< Unique name of the element */
    Explicit<std::string_view> label;                      /*!< Text rendered next to the input */
    Explicit<ParamType> speed;                             /*!< Speed of value change on drag */
    Explicit<ParamType> min;                               /*!< Minimum allowed value */
    Explicit<ParamType> max;                               /*!< Maximum allowed value */
    T value{};                                             /*!< Initial value */
    std::string format = drag_details::defaultFormat<T>(); /*!< Text format for value */
    bool persistent = false;                               /*!< Allow state saving to disk */
  };
  /**
   * Construct DragInput
   * @param config construction args @see DragInput::Config
   */
  explicit DragInput(Config &&config);

  /**
   * Construct DragInput.
   * @param elementName ID of the DragInput
   * @param labelText text drawn next to the input
   * @param valueSpeed frequency of value change based on mouse movement distance
   * @param minValue minimum allowed value
   * @param maxValue maximum allowed value
   * @param initialValue starting value
   * @param persistent allow state saving to disk
   * @param format format for formatting value to string
   */
  DragInput(std::string_view elementName, std::string_view labelText, ParamType valueSpeed, ParamType minValue, ParamType maxValue,
            T initialValue = T{}, Persistent persistent = Persistent::No, std::string numberFormat = drag_details::defaultFormat<T>());

  /**
   * Get movement speed.
   * @return movement speed
   */
  [[nodiscard]] ParamType getSpeed() const { return speed; }
  /**
   * Set movement speed.
   * @param speed new speed
   */
  void setSpeed(ParamType newSpeed) { speed = newSpeed; }
  /**
   * Get min drag value.
   * @return min drag value
   */
  [[nodiscard]] ParamType getMin() const { return min; }
  /**
   * Set min drag value.
   * @param min new min drag value
   */
  void setMin(ParamType newMin) { min = newMin; }
  /**
   * Get max drag value.
   * @return max drag value
   */
  [[nodiscard]] ParamType getMax() const { return max; }
  /**
   * Set max drag value.
   * @param max new min drag value
   */
  void setMax(ParamType newMax) { max = newMax; }

  [[nodiscard]] std::string_view getFormat() const { return format; }

  void setFormat(std::string newFormat) { format = std::move(newFormat); }

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::NavHighlight, ColorOf::Border, ColorOf::BorderShadow, ColorOf::SliderGrab,
               ColorOf::SliderGrabActive>
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

  ParamType speed;
  ParamType min;
  ParamType max;
  std::string format;
};

/**
 * @brief DragInput with labels next to each component.
 * @tparam T underlying value
 */
template<OneOf<PF_IMGUI_LABELEDDRAG_TYPE_LIST> T>
class PF_IMGUI_EXPORT LabeledDragInput : public DragInput<T> {
 public:
  using ParamType = typename DragInput<T>::ParamType;
  using DragInput<T>::ComponentCount;
  /**
   * @brief Struct for construction of LabeledDragInput.
   */
  struct Config {
    using Parent = LabeledDragInput;
    Explicit<std::string_view> name;                                   /*!< Unique name of the element */
    Explicit<std::string_view> label;                                  /*!< Text rendered next to the input */
    Explicit<ParamType> speed;                                         /*!< Speed of value change on drag */
    Explicit<ParamType> min;                                           /*!< Minimum allowed value */
    Explicit<ParamType> max;                                           /*!< Maximum allowed value */
    Explicit<std::array<std::string, ComponentCount>> componentLabels; /*!< Labels for drag's components */
    Explicit<std::array<Color, ComponentCount>> componentLabelColors;  /*!< Color for labels for drag's components */
    T value{};                                                         /*!< Initial value */
    std::string format = drag_details::defaultFormat<T>();             /*!< Text format for value */
    bool persistent = false;                                           /*!< Allow state saving to disk */
  };
  /**
   * Construct DragInput
   * @param config construction args @see DragInput::Config
   */
  explicit LabeledDragInput(Config &&config);

  /**
   * Construct DragInput.
   * @param elementName ID of the DragInput
   * @param labelText text drawn next to the input
   * @param valueSpeed frequency of value change based on mouse movement distance
   * @param minValue minimum allowed value
   * @param maxValue maximum allowed value
   * @param componentLabels Labels for drag's components
   * @param initialValue starting value
   * @param persistent allow state saving to disk
   * @param format format for formatting value to string
   */
  LabeledDragInput(std::string_view elementName, std::string_view labelText, ParamType valueSpeed, ParamType minValue, ParamType maxValue,
                   std::array<std::string, ComponentCount> componentLabels, std::array<Color, ComponentCount> componentLabelColors,
                   T initialValue = T{}, Persistent persistent = Persistent::No,
                   std::string numberFormat = drag_details::defaultFormat<T>());

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

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
DragInput<T>::DragInput(DragInput::Config &&config)
    : DragInput(config.name, config.label, config.speed, config.min, config.max, config.value,
                config.persistent ? Persistent::Yes : Persistent::No, config.format) {}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
DragInput<T>::DragInput(std::string_view elementName, std::string_view labelText, drag_details::UnderlyingType<T> valueSpeed,
                        drag_details::UnderlyingType<T> minValue, drag_details::UnderlyingType<T> maxValue, T initialValue,
                        Persistent persistent, std::string numberFormat)
    : ItemElement(elementName), Savable(persistent), DragSource<T>(false), DropTarget<T>(false), label(std::string{labelText}),
      value(initialValue), speed(valueSpeed), min(minValue), max(maxValue), format(std::move(numberFormat)) {}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
toml::table DragInput<T>::toToml() const {
  const auto val = *value;
  if constexpr (OneOf<T, PF_IMGUI_DRAG_RANGE_TYPE_LIST>) {
    return toml::table{{"value", toml::array{val.start, val.end}}};
  } else if constexpr (OneOf<T, PF_IMGUI_DRAG_GLM_TYPE_LIST>) {
    return toml::table{{"value", serializeGlmVec(val)}};
  } else {
    return toml::table{{"value", val}};
  }
}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
void DragInput<T>::setFromToml(const toml::table &src) {
  if constexpr (OneOf<T, PF_IMGUI_DRAG_RANGE_TYPE_LIST>) {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        if (newVal->size() != 2) { return; }
        auto range = T{};
        if (auto newRangeStart = newVal->get(0)->value<ParamType>(); newRangeStart.has_value()) { range.start = newRangeStart.value(); }
        if (auto newRangeEnd = newVal->get(1)->value<ParamType>(); newRangeEnd.has_value()) { range.end = newRangeEnd.value(); }
        *value.modify() = range;
      }
    }
  } else if constexpr (OneOf<T, PF_IMGUI_DRAG_GLM_TYPE_LIST>) {
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

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
void DragInput<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  bool valueChanged = false;
  const auto address = &Prop_value(value);
  const auto flags = ImGuiSliderFlags_AlwaysClamp;

  ImGuiDataType_ dataType;
  if constexpr (OneOf<T, PF_IMGUI_DRAG_FLOAT_TYPE_LIST>) {
    dataType = ImGuiDataType_Float;
  } else {
    dataType = ImGuiDataType_S32;
  }

  if constexpr (OneOf<T, int, float>) {
    valueChanged = ImGui::DragScalar(label->get().c_str(), dataType, address, static_cast<float>(speed), &min, &max, format.c_str(), flags);
  }
  if constexpr (OneOf<T, PF_IMGUI_DRAG_GLM_TYPE_LIST>) {
    valueChanged = ImGui::DragScalarN(label->get().c_str(), dataType, glm::value_ptr(*address), ComponentCount, static_cast<float>(speed),
                                      &min, &max, format.c_str(), flags);
  }

  if constexpr (std::same_as<T, math::Range<int>>) {
    valueChanged = ImGui::DragIntRange2(label->get().c_str(), &address->start, &address->end, static_cast<float>(speed), min, max,
                                        format.c_str(), nullptr, flags);
  }
  if constexpr (std::same_as<T, math::Range<float>>) {
    valueChanged =
        ImGui::DragFloatRange2(label->get().c_str(), &address->start, &address->end, speed, min, max, format.c_str(), nullptr, flags);
  }
  DragSource<T>::drag(*value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *value.modify() = *drop;
    return;
  }
  if (valueChanged) { Prop_triggerListeners(value); }
}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
const T &DragInput<T>::getValue() const {
  return *value;
}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
void DragInput<T>::setValue(const T &newValue) {
  *Prop_modify(value) = newValue;
}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
Subscription DragInput<T>::addValueListenerImpl(std::function<void(const T &)> listener) {
  return value.addListener(std::move(listener));
}

template<OneOf<PF_IMGUI_LABELEDDRAG_TYPE_LIST> T>
LabeledDragInput<T>::LabeledDragInput(LabeledDragInput::Config &&config)
    : LabeledDragInput(config.name, config.label, config.speed, config.min, config.max, config.componentLabels, config.componentLabelColors,
                       config.value, config.persistent ? Persistent::Yes : Persistent::No, config.format) {}

template<OneOf<PF_IMGUI_LABELEDDRAG_TYPE_LIST> T>
LabeledDragInput<T>::LabeledDragInput(std::string_view elementName, std::string_view labelText, LabeledDragInput::ParamType valueSpeed,
                                      LabeledDragInput::ParamType minValue, LabeledDragInput::ParamType maxValue,
                                      std::array<std::string, ComponentCount> componentLabels,
                                      std::array<Color, ComponentCount> componentLabelColors, T initialValue, Persistent persistent,
                                      std::string numberFormat)
    : DragInput<T>{elementName, labelText, valueSpeed, minValue, maxValue, initialValue, persistent, numberFormat},
      labels{componentLabels} {
  loadCstrLabels();
  setComponentLabelColors(componentLabelColors);
}

template<OneOf<PF_IMGUI_LABELEDDRAG_TYPE_LIST> T>
void LabeledDragInput<T>::setComponentLabels(std::array<std::string, ComponentCount> componentLabels) {
  labels = componentLabels;
  loadCstrLabels();
}

template<OneOf<PF_IMGUI_LABELEDDRAG_TYPE_LIST> T>
void LabeledDragInput<T>::setComponentLabelColors(std::array<Color, ComponentCount> componentColors) {
  for (std::size_t i = 0; i < ComponentCount; ++i) { labelColors[i] = static_cast<ImU32>(componentColors[i]); }
}

template<OneOf<PF_IMGUI_LABELEDDRAG_TYPE_LIST> T>
void LabeledDragInput<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = this->color.applyScoped();
  [[maybe_unused]] auto styleScoped = this->style.applyScoped();
  [[maybe_unused]] auto fontScoped = this->font.applyScopedIfNotDefault();
  bool valueChanged = false;
  const auto address = &PropertyOwner::Prop_value(this->value);
  const auto flags = ImGuiSliderFlags_AlwaysClamp;

  ImGuiDataType_ dataType;
  if constexpr (OneOf<T, PF_IMGUI_DRAG_FLOAT_TYPE_LIST>) {
    dataType = ImGuiDataType_Float;
  } else {
    dataType = ImGuiDataType_S32;
  }

  if constexpr (OneOf<T, int, float>) {
    valueChanged = ImGui::LabeledDragScalar(this->label->get().c_str(), dataType, address, static_cast<float>(this->speed), &this->min,
                                            &this->max, this->format.c_str(), labelsCstr[0], labelColors[0], flags);
  }
  if constexpr (OneOf<T, PF_IMGUI_DRAG_GLM_TYPE_LIST>) {
    valueChanged = ImGui::LabeledDragScalarN(this->label->get().c_str(), dataType, glm::value_ptr(*address), ComponentCount,
                                             static_cast<float>(this->speed), &this->min, &this->max, this->format.c_str(),
                                             labelsCstr.data(), labelColors.data(), flags);
  }

  DragSource<T>::drag(*this->value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *this->value.modify() = *drop;
    return;
  }
  if (valueChanged) { PropertyOwner::Prop_triggerListeners(this->value); }
}

extern template class DragInput<float>;
extern template class DragInput<glm::vec2>;
extern template class DragInput<glm::vec3>;
extern template class DragInput<glm::vec4>;
extern template class DragInput<math::Range<float>>;
extern template class DragInput<int>;
extern template class DragInput<glm::ivec2>;
extern template class DragInput<glm::ivec3>;
extern template class DragInput<glm::ivec4>;
extern template class DragInput<math::Range<int>>;

extern template class LabeledDragInput<float>;
extern template class LabeledDragInput<glm::vec2>;
extern template class LabeledDragInput<glm::vec3>;
extern template class LabeledDragInput<glm::vec4>;
extern template class LabeledDragInput<int>;
extern template class LabeledDragInput<glm::ivec2>;
extern template class LabeledDragInput<glm::ivec3>;
extern template class LabeledDragInput<glm::ivec4>;

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_DRAGINPUT_H
