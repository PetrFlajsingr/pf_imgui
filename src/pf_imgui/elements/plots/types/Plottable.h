//
// Created by xflajs00 on 18.09.2022.
//

#ifndef PF_IMGUI_PLOTS_TYPES_PLOTTABLE_H
#define PF_IMGUI_PLOTS_TYPES_PLOTTABLE_H

#include <concepts>

namespace pf::ui::ig::plot_type {
/**
 * @brief A type which can be rendered using Plot and its descendants.
 */
template<typename T>
concept Plottable = std::convertible_to<T, double>;
}  // namespace pf::ui::ig::plot_type

#endif  //PF_IMGUI_PLOTS_TYPES_PLOTTABLE_H
