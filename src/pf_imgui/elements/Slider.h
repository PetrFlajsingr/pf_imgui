/**
 * @file Slider.h
 * @brief Slider element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_SLIDER_H
#define PF_IMGUI_ELEMENTS_SLIDER_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
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

namespace details {
/**
 * Types using float as underlying value.
 */
#define IMGUI_SLIDER_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4
/**
 * Types using int as underlying value.
 */
#define IMGUI_SLIDER_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * Supported glm types.
 */
#define IMGUI_SLIDER_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * All types supported by Slider.
 */
#define IMGUI_SLIDER_TYPE_LIST IMGUI_SLIDER_FLOAT_TYPE_LIST, IMGUI_SLIDER_INT_TYPE_LIST
/**
 * Detection for underlying type.
 */
template<OneOf<IMGUI_SLIDER_TYPE_LIST> T>
using SliderMinMaxType = std::conditional_t<OneOf<T, IMGUI_SLIDER_FLOAT_TYPE_LIST>, float, int>;

/**
 * Default formats for underlying types.
 * @tparam T underlying type
 * @return format
 */
template<typename T>
constexpr const char *defaultSliderFormat() {
  if constexpr (OneOf<T, IMGUI_SLIDER_FLOAT_TYPE_LIST>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
}  // namespace details

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
template<OneOf<IMGUI_SLIDER_TYPE_LIST> T>
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
  using MinMaxType = details::SliderMinMaxType<T>;
  /**
   * @brief Struct for construction of Slider.
   */
  struct Config {
    using Parent = Slider;
    std::string_view name;                                           /*!< Unique name of the element */
    std::string_view label;                                          /*!< Text rendered next to the element */
    MinMaxType min;                                                  /*!< Minimum allowed value */
    MinMaxType max;                                                  /*!< Maximum allowed value */
    T value{};                                                       /*!< Initial value */
    std::string format = details::defaultSliderFormat<MinMaxType>(); /*!< Format string for value rendering */
    Persistent persistent = Persistent::No;                          /*!< Allow state saving to disk */
  };
  /**
   * Construct Slider
   * @param config construction args @see Slider::Config
   */
  explicit Slider(Config &&config)
      : ItemElement(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable<T>(config.value),
        Savable(config.persistent), DragSource<T>(false), DropTarget<T>(false), min(config.min), max(config.max),
        format(std::move(config.format)) {}
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
         Persistent persistent = Persistent::No, std::string format = details::defaultSliderFormat<MinMaxType>())
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(value),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), min(min), max(max), format(std::move(format)) {
  }

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

 protected:
  void unserialize_impl(const toml::table &src) override {
    if constexpr (OneOf<T, IMGUI_SLIDER_GLM_TYPE_LIST>) {
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
    if constexpr (OneOf<T, IMGUI_SLIDER_GLM_TYPE_LIST>) {
      return toml::table{{"value", serializeGlmVec(value)}};
    } else {
      return toml::table{{"value", value}};
    }
  }

  void renderImpl() override {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    auto valueChanged = false;
    const auto address = ValueObservable<T>::getValueAddress();
    const auto flags = ImGuiSliderFlags_AlwaysClamp;
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImGui::SliderFloat(getLabel().c_str(), address, min, max, format.c_str(), flags);
    }
    if constexpr (std::same_as<T, glm::vec2>) {
      valueChanged = ImGui::SliderFloat2(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str(), flags);
    }
    if constexpr (std::same_as<T, glm::vec3>) {
      valueChanged = ImGui::SliderFloat3(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str(), flags);
    }
    if constexpr (std::same_as<T, glm::vec4>) {
      valueChanged = ImGui::SliderFloat4(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str(), flags);
    }
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImGui::SliderInt(getLabel().c_str(), address, min, max, format.c_str(), flags);
    }
    if constexpr (std::same_as<T, glm::ivec2>) {
      valueChanged = ImGui::SliderInt2(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str(), flags);
    }
    if constexpr (std::same_as<T, glm::ivec3>) {
      valueChanged = ImGui::SliderInt3(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str(), flags);
    }
    if constexpr (std::same_as<T, glm::ivec4>) {
      valueChanged = ImGui::SliderInt4(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str(), flags);
    }
    DragSource<T>::drag(ValueObservable<T>::getValue());
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
      ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
      return;
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }

 private:
  MinMaxType min;
  MinMaxType max;
  std::string format;
};

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
