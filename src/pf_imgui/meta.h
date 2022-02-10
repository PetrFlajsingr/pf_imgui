/**
 * @file meta.h
 * @brief Meta helpers for pf_imgui.
 * @author Petr Flajšingr
 * @date 12.4.21
 */

#ifndef PF_IMGUI_META_H
#define PF_IMGUI_META_H

#include <pf_common/specializations.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {
/*
 template<typename T>
requires requires { typename T::Parent; }
std::derived_from<Element> auto &createChildAtIndex(std::size_t index, T &&config) requires(
   std::derived_from<typename T::Parent, Element> &&std::constructible_from<typename T::Parent, T>)
 */

template<typename T>
concept ElementConstructConfig = requires {
  typename T::Parent;
}
&&std::derived_from<typename T::Parent, Element> &&std::constructible_from<typename T::Parent, T>;

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

struct Dummy {};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_META_H
