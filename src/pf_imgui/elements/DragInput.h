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
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/math/Range.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
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
namespace details {
/**
 * Types with float as underlying value.
 */
#define IMGUI_DRAG_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4, math::Range<float>
/**
 * Types with int as underlying value.
 */
#define IMGUI_DRAG_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4, math::Range<int>
/**
 * Used glm types.
 */
#define IMGUI_DRAG_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * Used range types.
 */
#define IMGUI_DRAG_RANGE_TYPE_LIST math::Range<int>, math::Range<float>
/**
 * All allowed types for DragInput
 */
#define IMGUI_DRAG_TYPE_LIST IMGUI_DRAG_FLOAT_TYPE_LIST, IMGUI_DRAG_INT_TYPE_LIST
/**
 * Underlying type of supported types.
 */
template<typename T>
using DragInputUnderlyingType = std::conditional_t<OneOf<T, IMGUI_DRAG_FLOAT_TYPE_LIST>, float, int>;
/**
 * Default formatting string for supported types.
 * @tparam T type to based format on
 * @return printf like format for numbers
 */
template<typename T>
constexpr const char *defaultDragFormat() {
  if constexpr (OneOf<T, IMGUI_DRAG_FLOAT_TYPE_LIST> || std::same_as<T, math::Range<float>>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
}  // namespace details

/**
 * @brief Numeric input supporting either keyboard input or mouse drag.
 *
 * Type of the DragInput is based on underlying types. There is a separate input for each scalar part of type T.
 *
 * Supported types: int, float, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4, math::Range<int>, math::Range<float>
 *
 * @tparam T Underlying type
 */
template<OneOf<IMGUI_DRAG_TYPE_LIST> T>
class PF_IMGUI_EXPORT DragInput
    : public ItemElement,
      public ValueObservable<T>,
      public Labellable,
      public Savable,
      public DragSource<T>,
      public DropTarget<T>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::DragDropTarget,
                               style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::NavHighlight,
                               style::ColorOf::Border, style::ColorOf::BorderShadow, style::ColorOf::SliderGrab,
                               style::ColorOf::SliderGrabActive>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  using ParamType = details::DragInputUnderlyingType<T>;

  /**
   * @brief Struct for construction of DragInput.
   */
  struct Config {
    using Parent = DragInput;
    std::string_view name;                                /*!< Unique name of the element */
    std::string_view label;                               /*!< Text rendered next to the input */
    ParamType speed;                                      /*!< Speed of value change on drag */
    ParamType min;                                        /*!< Minimum allowed value */
    ParamType max;                                        /*!< Maximum allowed value */
    T value{};                                            /*!< Initial value */
    std::string format = details::defaultDragFormat<T>(); /*!< Text format for value */
    bool persistent = false;                              /*!< Allow state saving to disk */
  };
  /**
   * Construct DragInput
   * @param config construction args @see DragInput::Config
   */
  explicit DragInput(Config &&config)
      : ItemElement(std::string{config.name}), ValueObservable<T>(config.value), Labellable(std::string{config.label}),
        Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<T>(false), DropTarget<T>(false),
        speed(config.speed), min(config.min), max(config.max), format(std::move(config.format)) {}

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
            T value = T{}, Persistent persistent = Persistent::No, std::string format = details::defaultDragFormat<T>())
      : ItemElement(elementName), ValueObservable<T>(value), Labellable(label),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), speed(speed), min(min), max(max),
        format(std::move(format)) {}

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

 protected:
  void unserialize_impl(const toml::table &src) override {
    if constexpr (OneOf<T, IMGUI_DRAG_RANGE_TYPE_LIST>) {
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
    } else if constexpr (OneOf<T, IMGUI_DRAG_GLM_TYPE_LIST>) {
      if (auto newValIter = src.find("value"); newValIter != src.end()) {
        if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
          const auto vecValue = safeDeserializeGlmVec<T>(*newVal);
          if (vecValue.has_value()) { ValueObservable<T>::setValueAndNotifyIfChanged(vecValue.value()); }
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

  [[nodiscard]] toml::table serialize_impl() const override {
    const auto value = ValueObservable<T>::getValue();
    if constexpr (OneOf<T, IMGUI_DRAG_RANGE_TYPE_LIST>) {
      return toml::table{{"value", toml::array{value.start, value.end}}};
    } else if constexpr (OneOf<T, IMGUI_DRAG_GLM_TYPE_LIST>) {
      return toml::table{{"value", serializeGlmVec(value)}};
    } else {
      return toml::table{{"value", value}};
    }
  }

  void renderImpl() override {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    bool valueChanged = false;
    const auto address = ValueObservable<T>::getValueAddress();
    const auto flags = ImGuiSliderFlags_AlwaysClamp;

    ImGuiDataType_ dataType;
    if constexpr (OneOf<T, IMGUI_DRAG_FLOAT_TYPE_LIST>) {
      dataType = ImGuiDataType_Float;
    } else {
      dataType = ImGuiDataType_S32;
    }

    if constexpr (OneOf<T, int, float>) {
      valueChanged = ImGui::DragScalar(getLabel().c_str(), dataType, address, static_cast<float>(speed), &min, &max,
                                       format.c_str(), flags);
    }
    if constexpr (OneOf<T, IMGUI_DRAG_GLM_TYPE_LIST>) {
      valueChanged = ImGui::DragScalarN(getLabel().c_str(), dataType, glm::value_ptr(*address), T::length(),
                                        static_cast<float>(speed), &min, &max, format.c_str(), flags);
    }

    if constexpr (std::same_as<T, math::Range<int>>) {
      valueChanged = ImGui::DragIntRange2(getLabel().c_str(), &address->start, &address->end, static_cast<float>(speed),
                                          min, max, format.c_str(), nullptr, flags);
    }
    if constexpr (std::same_as<T, math::Range<float>>) {
      valueChanged = ImGui::DragFloatRange2(getLabel().c_str(), &address->start, &address->end, speed, min, max,
                                            format.c_str(), nullptr, flags);
    }
    DragSource<T>::drag(ValueObservable<T>::getValue());
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
      ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
      return;
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }

 private:
  ParamType speed;
  ParamType min;
  ParamType max;
  std::string format;
};

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
