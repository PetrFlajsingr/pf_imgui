//
// Created by xflajs00 on 11.05.2022.
//

#ifndef PF_IMGUI_ELEMENTS_DETAILS_INPUT_H
#define PF_IMGUI_ELEMENTS_DETAILS_INPUT_H

#include <concepts>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <pf_common/concepts/OneOf.h>
#include <type_traits>

namespace pf::ui::ig::input_details {
/**
 * Underlying types supporting step.
 */
#define PF_IMGUI_INPUT_STEP_TYPE_LIST float, double, int
/**
 * Types using float as underlying value.
 */
#define PF_IMGUI_INPUT_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4
/**
 * Types using double as underlying value.
 */
#define PF_IMGUI_INPUT_DOUBLE_TYPE_LIST double
/**
 * Types using int as underlying value.
 */
#define PF_IMGUI_INPUT_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * Supported glm types.
 */
#define PF_IMGUI_INPUT_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * All types supported by Input.
 */
#define PF_IMGUI_INPUT_TYPE_LIST                                                                                       \
  PF_IMGUI_INPUT_FLOAT_TYPE_LIST, PF_IMGUI_INPUT_INT_TYPE_LIST, PF_IMGUI_INPUT_DOUBLE_TYPE_LIST

/**
 * Underlying type of supported types.
 */
template<OneOf<PF_IMGUI_INPUT_TYPE_LIST> T>
using UnderlyingType = std::conditional_t<OneOf<T, PF_IMGUI_INPUT_FLOAT_TYPE_LIST>, float,
                                          std::conditional_t<OneOf<T, PF_IMGUI_INPUT_INT_TYPE_LIST>, int, double>>;
/**
 * Default formatting string for supported types.
 * @tparam T type to based format on
 * @return printf like format for numbers
 */
template<typename T>
constexpr const char *defaultFormat() {
  if constexpr (std::same_as<UnderlyingType<T>, float>) {
    return "%.3f";
  } else if constexpr (std::same_as<UnderlyingType<T>, double>) {
    return "%.6f";
  } else {
    return "%d";
  }
}

}  // namespace pf::ui::ig::input_details

#endif  //PF_IMGUI_ELEMENTS_DETAILS_INPUT_H
