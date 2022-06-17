/**
 * @file meta.h
 * @brief Meta helpers for pf_imgui.
 * @author Petr Flajšingr
 * @date 12.4.21
 */

#ifndef PF_IMGUI_META_H
#define PF_IMGUI_META_H

#include <pf_common/specializations.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

template<typename T>
concept ElementConstructConfig = requires { typename T::Parent; }
    && std::derived_from<typename T::Parent, Element> && std::constructible_from<typename T::Parent, T>;

template<typename T>
concept ConfigConstructible = requires { typename T::Config; } && ElementConstructConfig<typename T::Config>;

/**
 * Check if the type is a specialization of ValueObservable.
 * @tparam T type to be checked
 */
template<typename T>
constexpr auto IsValueObservable = derived_specialization_of<T, ValueObservable>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_META_H
