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
#include <pf_imgui/elements/details/DragInputDetails.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
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
class PF_IMGUI_EXPORT DragInput : public ItemElement,
                                  public ValueObservable<T>,
                                  public Savable,
                                  public DragSource<T>,
                                  public DropTarget<T> {
 public:
  using ParamType = drag_details::UnderlyingType<T>;

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
   * @param label text drawn next to the input
   * @param speed frequency of value change based on mouse movement distance
   * @param min minimum allowed value
   * @param max maximum allowed value
   * @param persistent allow state saving to disk
   * @param value starting value
   * @param format format for formatting value to string
   */
  DragInput(const std::string &elementName, const std::string &label, ParamType speed, ParamType min, ParamType max,
            T value = T{}, Persistent persistent = Persistent::No,
            std::string format = drag_details::defaultFormat<T>());

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

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::FrameBackground,
               ColorOf::FrameBackgroundHovered, ColorOf::FrameBackgroundActive, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow, ColorOf::SliderGrab, ColorOf::SliderGrabActive>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Label label;

 protected:
  void renderImpl() override;

 private:
  ParamType speed;
  ParamType min;
  ParamType max;
  std::string format;
};

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
DragInput<T>::DragInput(DragInput::Config &&config)
    : ItemElement(std::string{config.name.value}), ValueObservable<T>(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<T>(false), DropTarget<T>(false),
      label(std::string{config.label.value}), speed(config.speed), min(config.min), max(config.max),
      format(std::move(config.format)) {}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
DragInput<T>::DragInput(const std::string &elementName, const std::string &label, drag_details::UnderlyingType<T> speed,
                        drag_details::UnderlyingType<T> min, drag_details::UnderlyingType<T> max, T value,
                        Persistent persistent, std::string format)
    : ItemElement(elementName), ValueObservable<T>(value),
      Savable(persistent), DragSource<T>(false), DropTarget<T>(false), label(label), speed(speed), min(min), max(max),
      format(std::move(format)) {}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
toml::table DragInput<T>::toToml() const {
  const auto value = ValueObservable<T>::getValue();
  if constexpr (OneOf<T, PF_IMGUI_DRAG_RANGE_TYPE_LIST>) {
    return toml::table{{"value", toml::array{value.start, value.end}}};
  } else if constexpr (OneOf<T, PF_IMGUI_DRAG_GLM_TYPE_LIST>) {
    return toml::table{{"value", serializeGlmVec(value)}};
  } else {
    return toml::table{{"value", value}};
  }
}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
void DragInput<T>::setFromToml(const toml::table &src) {
  if constexpr (OneOf<T, PF_IMGUI_DRAG_RANGE_TYPE_LIST>) {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        if (newVal->size() != 2) { return; }
        auto range = T{};
        if (auto newRangeStart = newVal->get(0)->value<ParamType>(); newRangeStart.has_value()) {
          range.start = newRangeStart.value();
        }
        if (auto newRangeEnd = newVal->get(1)->value<ParamType>(); newRangeEnd.has_value()) {
          range.end = newRangeEnd.value();
        }
        ValueObservable<T>::setValueAndNotifyIfChanged(range);
      }
    }
  } else if constexpr (OneOf<T, PF_IMGUI_DRAG_GLM_TYPE_LIST>) {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        const auto vecValue = safeDeserializeGlmVec<T>(*newVal);
        if (vecValue.has_value()) { ValueObservable<T>::setValueAndNotifyIfChanged(*vecValue); }
      }
    }
  } else {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.value<T>(); newVal.has_value()) {
        ValueObservable<T>::setValueAndNotifyIfChanged(*newVal);
      }
    }
  }
}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
void DragInput<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  bool valueChanged = false;
  const auto address = ValueObservable<T>::getValueAddress();
  const auto flags = ImGuiSliderFlags_AlwaysClamp;

  ImGuiDataType_ dataType;
  if constexpr (OneOf<T, PF_IMGUI_DRAG_FLOAT_TYPE_LIST>) {
    dataType = ImGuiDataType_Float;
  } else {
    dataType = ImGuiDataType_S32;
  }

  if constexpr (OneOf<T, int, float>) {
    valueChanged = ImGui::DragScalar(label.get().c_str(), dataType, address, static_cast<float>(speed), &min, &max,
                                     format.c_str(), flags);
  }
  if constexpr (OneOf<T, PF_IMGUI_DRAG_GLM_TYPE_LIST>) {
    valueChanged = ImGui::DragScalarN(label.get().c_str(), dataType, glm::value_ptr(*address), T::length(),
                                      static_cast<float>(speed), &min, &max, format.c_str(), flags);
  }

  if constexpr (std::same_as<T, math::Range<int>>) {
    valueChanged = ImGui::DragIntRange2(label.get().c_str(), &address->start, &address->end, static_cast<float>(speed),
                                        min, max, format.c_str(), nullptr, flags);
  }
  if constexpr (std::same_as<T, math::Range<float>>) {
    valueChanged = ImGui::DragFloatRange2(label.get().c_str(), &address->start, &address->end, speed, min, max,
                                          format.c_str(), nullptr, flags);
  }
  DragSource<T>::drag(ValueObservable<T>::getValue());
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
    return;
  }
  if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
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

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_DRAGINPUT_H
