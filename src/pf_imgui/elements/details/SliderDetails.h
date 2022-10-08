//
// Created by xflajs00 on 11.05.2022.
//

#ifndef PF_IMGUI_ELEMENTS_DETAILS_SLIDER_H
#define PF_IMGUI_ELEMENTS_DETAILS_SLIDER_H

#include <concepts>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <pf_common/concepts/OneOf.h>
#include <type_traits>

namespace pf::ui::ig::slider_details {
/**
 * Types using float as underlying value.
 */
#define PF_IMGUI_SLIDER_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4
/**
 * Types using int as underlying value.
 */
#define PF_IMGUI_SLIDER_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * Supported glm types.
 */
#define PF_IMGUI_SLIDER_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * All types supported by Slider.
 */
#define PF_IMGUI_SLIDER_TYPE_LIST PF_IMGUI_SLIDER_FLOAT_TYPE_LIST, PF_IMGUI_SLIDER_INT_TYPE_LIST
/**
 * All types supported by LabeledSlider.
 */
#define PF_IMGUI_LABELEDSLIDER_TYPE_LIST PF_IMGUI_SLIDER_FLOAT_TYPE_LIST, PF_IMGUI_SLIDER_INT_TYPE_LIST
/**
 * Detection for underlying type.
 */
template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
using MinMaxType = std::conditional_t<OneOf<T, PF_IMGUI_SLIDER_FLOAT_TYPE_LIST>, float, int>;

/**
 * Default formats for underlying types.
 * @tparam T underlying type
 * @return format
 */
template<typename T>
constexpr const char *defaultFormat() {
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_FLOAT_TYPE_LIST>) {
    return "%.3f";
  } else {
    return "%d";
  }
}

template<OneOf<PF_IMGUI_SLIDER_TYPE_LIST> T>
consteval std::size_t getComponentCount() {
  if constexpr (OneOf<T, PF_IMGUI_SLIDER_GLM_TYPE_LIST>) { return T::length(); }
  return 1;
}
}  // namespace pf::ui::ig::slider_details

#endif  //PF_IMGUI_ELEMENTS_DETAILS_SLIDER_H
