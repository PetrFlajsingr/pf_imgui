//
// Created by xflajs00 on 11.05.2022.
//

#ifndef PF_IMGUI_ELEMENTS_DETAILS_DRAGINPUT_H
#define PF_IMGUI_ELEMENTS_DETAILS_DRAGINPUT_H

#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/math/Range.h>
#include <type_traits>

namespace pf::ui::ig::drag_details {
/**
 * Types with float as underlying value.
 */
#define PF_IMGUI_DRAG_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4, math::Range<float>
/**
 * Types with int as underlying value.
 */
#define PF_IMGUI_DRAG_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4, math::Range<int>
/**
 * Used glm types.
 */
#define PF_IMGUI_DRAG_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * Used range types.
 */
#define PF_IMGUI_DRAG_RANGE_TYPE_LIST math::Range<int>, math::Range<float>
/**
 * All allowed types for DragInput
 */
#define PF_IMGUI_DRAG_TYPE_LIST PF_IMGUI_DRAG_FLOAT_TYPE_LIST, PF_IMGUI_DRAG_INT_TYPE_LIST
/**
 * All allowed types for LabeledDragInput
 */
#define PF_IMGUI_LABELEDDRAG_TYPE_LIST PF_IMGUI_DRAG_GLM_TYPE_LIST, int, float
/**
 * Underlying type of supported types.
 */
template<typename T>
using UnderlyingType = std::conditional_t<OneOf<T, PF_IMGUI_DRAG_FLOAT_TYPE_LIST>, float, int>;
/**
 * Default formatting string for supported types.
 * @tparam T type to based format on
 * @return printf like format for numbers
 */
template<typename T>
constexpr const char *defaultFormat() {
  if constexpr (OneOf<T, PF_IMGUI_DRAG_FLOAT_TYPE_LIST> || std::same_as<T, math::Range<float>>) {
    return "%.3f";
  } else {
    return "%d";
  }
}

template<OneOf<PF_IMGUI_DRAG_TYPE_LIST> T>
consteval std::size_t getComponentCount() {
  if constexpr (OneOf<T, PF_IMGUI_DRAG_GLM_TYPE_LIST>) { return T::length(); }
  if constexpr (OneOf<T, PF_IMGUI_DRAG_RANGE_TYPE_LIST>) { return 2; }
  return 1;
}
}  // namespace pf::ui::ig::drag_details

#endif  //PF_IMGUI_ELEMENTS_DETAILS_DRAGINPUT_H
