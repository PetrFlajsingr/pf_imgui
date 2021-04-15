//
// Created by petr on 4/12/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_META_H
#define PF_IMGUI_SRC_PF_IMGUI_META_H

#include <pf_common/specializations.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

/**
 * Check if the type is a specialization of ValueObservable.
 * @tparam T type to be checked
 */
template<typename T>
constexpr auto IsValueObservable = derived_specialization_of<T, ValueObservable>;
/**
 * @brief Placeholder type to avoid compilation errors.
 */
struct CommonPlaceholder {
  using CreateType = int;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_META_H
