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
}// namespace details

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
class PF_IMGUI_EXPORT Slider : public ItemElement,
                               public Labellable,
                               public ValueObservable<T>,
                               public Savable,
                               public DragSource<T>,
                               public DropTarget<T> {
 public:
  using MinMaxType = details::SliderMinMaxType<T>;
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
      const auto tomlVec = src["value"].as_array();
      const auto vec = deserializeGlmVec<T>(*tomlVec);
      ValueObservable<T>::setValueAndNotifyIfChanged(vec);
    } else {
      ValueObservable<T>::setValueAndNotifyIfChanged(*src["value"].value<T>());
    }
  }

  toml::table serialize_impl() override {
    const auto value = ValueObservable<T>::getValue();
    if constexpr (OneOf<T, IMGUI_SLIDER_GLM_TYPE_LIST>) {
      return toml::table{{{"value", serializeGlmVec(value)}}};
    } else {
      return toml::table{{{"value", value}}};
    }
  }

  void renderImpl() override {
    auto valueChanged = false;
    const auto address = ValueObservable<T>::getValueAddress();
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImGui::SliderFloat(getLabel().c_str(), address, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec2>) {
      valueChanged = ImGui::SliderFloat2(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec3>) {
      valueChanged = ImGui::SliderFloat3(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec4>) {
      valueChanged = ImGui::SliderFloat4(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str());
    }
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImGui::SliderInt(getLabel().c_str(), address, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec2>) {
      valueChanged = ImGui::SliderInt2(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec3>) {
      valueChanged = ImGui::SliderInt3(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec4>) {
      valueChanged = ImGui::SliderInt4(getLabel().c_str(), glm::value_ptr(*address), min, max, format.c_str());
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
}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_SLIDER_H
