/**
 * @file ConfigConstruction.h
 * @brief Concepts for config construction.
 * @author Petr Flajšingr
 * @date 4.9.22
 */

#ifndef PF_IMGUI_CONCEPTS_CONFIGCONSTRUCTION_H
#define PF_IMGUI_CONCEPTS_CONFIGCONSTRUCTION_H

#include <pf_imgui/interface/ElementWithID.h>

namespace pf::ui::ig {

template<typename T>
concept ElementConstructConfig =
    requires { typename T::Parent; } && std::derived_from<typename T::Parent, Element> && std::constructible_from<typename T::Parent, T>;

template<typename T>
concept ConfigConstructible = requires { typename T::Config; } && ElementConstructConfig<typename T::Config>;

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_CONCEPTS_CONFIGCONSTRUCTION_H
