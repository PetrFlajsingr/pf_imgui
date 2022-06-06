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
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/details/SliderDetails.h>
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

/**
 * @brief Slider supporting multiple types.
 *
 * Supported types: int, float, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
 *
 * If the type is not a scalar, each scalar part of the structure gets its own slider.
 * @tparam T underlying type
 *
 * @todo: ToStringConvertible support
 */
template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
class PF_IMGUI_EXPORT Slider
    : public ItemElement,
      public Labellable,
      public ValueObservable<T>,
      public Savable,
      public DragSource<T>,
      public DropTarget<T>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::DragDropTarget,
                               style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::DragDropTarget,
                               style::ColorOf::SliderGrab, style::ColorOf::SliderGrabActive,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  using MinMaxType = slider_details::MinMaxType<T>;
  /**
   * @brief Struct for construction of Slider.
   */
  struct Config {
    using Parent = Slider;
    std::string_view name;                                            /*!< Unique name of the element */
    std::string_view label;                                           /*!< Text rendered next to the element */
    MinMaxType min;                                                   /*!< Minimum allowed value */
    MinMaxType max;                                                   /*!< Maximum allowed value */
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
   * @param label text rendered next to the slider
   * @param min min value
   * @param max max value
   * @param value starting value
   * @param persistent enable state saving to disk
   * @param format printf-like format for rendering value over slider
   */
  Slider(const std::string &elementName, const std::string &label, MinMaxType min, MinMaxType max, T value = T{},
         Persistent persistent = Persistent::No, std::string format = slider_details::defaultFormat<MinMaxType>());

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

 protected:
  void renderImpl() override;

 private:
  MinMaxType min;
  MinMaxType max;
  std::string format;
};

template<OneOf<float, glm::vec2, glm::vec3, glm::vec4, int, glm::ivec2, glm::ivec3, glm::ivec4> T>
Slider<T>::Slider(Slider::Config &&config)
    : ItemElement(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable<T>(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<T>(false), DropTarget<T>(false),
      min(config.min), max(config.max), format(std::move(config.format)) {}

template<OneOf<float, glm::vec2, glm::vec3, glm::vec4, int, glm::ivec2, glm::ivec3, glm::ivec4> T>
Slider<T>::Slider(const std::string &elementName, const std::string &label, Slider::MinMaxType min,
                  Slider::MinMaxType max, T value, Persistent persistent, std::string format)
    : ItemElement(elementName), Labellable(label), ValueObservable<T>(value),
      Savable(persistent), DragSource<T>(false), DropTarget<T>(false), min(min), max(max), format(std::move(format)) {}

template<OneOf<float, glm::vec2, glm::vec3, glm::vec4, int, glm::ivec2, glm::ivec3, glm::ivec4> T>
toml::table Slider<T>::toToml() const {
  const auto value = ValueObservable<T>::getValue();
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_GLM_TYPE_LIST>) {
    return toml::table{{"value", serializeGlmVec(value)}};
  } else {
    return toml::table{{"value", value}};
  }
}

template<OneOf<float, glm::vec2, glm::vec3, glm::vec4, int, glm::ivec2, glm::ivec3, glm::ivec4> T>
void Slider<T>::setFromToml(const toml::table &src) {
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_GLM_TYPE_LIST>) {
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

template<OneOf<float, glm::vec2, glm::vec3, glm::vec4, int, glm::ivec2, glm::ivec3, glm::ivec4> T>
void Slider<T>::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  auto valueChanged = false;
  const auto address = ValueObservable<T>::getValueAddress();
  const auto flags = ImGuiSliderFlags_AlwaysClamp;

  ImGuiDataType_ dataType;
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_FLOAT_TYPE_LIST>) {
    dataType = ImGuiDataType_Float;
  } else {
    dataType = ImGuiDataType_S32;
  }

  if constexpr (!OneOf<T, PF_IMGUI_SLIDER_GLM_TYPE_LIST>) {
    valueChanged = ImGui::SliderScalar(getLabel().c_str(), dataType, address, &min, &max, format.c_str(), flags);
  } else {
    valueChanged = ImGui::SliderScalarN(getLabel().c_str(), dataType, glm::value_ptr(*address), T::length(), &min, &max,
                                        format.c_str(), flags);
  }

  DragSource<T>::drag(ValueObservable<T>::getValue());
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
    return;
  }
  if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
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
