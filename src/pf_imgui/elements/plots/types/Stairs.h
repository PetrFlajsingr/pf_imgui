//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_STAIRS_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_STAIRS_H

#include <implot.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>

namespace pf::ui::ig::plot_type {

/**
 * @brief 2D plot where neighboring values are connected via 'stairs'.
 * Example:
 *    _
 *   | |
 *  _| |
 * |   |_
 * ___________
 */
class PF_IMGUI_EXPORT Stairs : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  /**
   * Construct Stairs.
   * @param elementName ID of the plot
   * @param caption text rendered above the plot
   */
  Stairs(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig::plot_type

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_STAIRS_H
