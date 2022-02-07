/**
 * @file Digital.h
 * @brief Digital graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_DIGITAL_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_DIGITAL_H

#include <implot.h>
#include <list>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>

namespace pf::ui::ig::plot_type {

/**
 * @brief 2D plot for digital data.
 */
class PF_IMGUI_EXPORT Digital : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  /**
   * Construct Digital.
   * @param elementName ID of the plot
   * @param caption text rendered above the plot
   */
  Digital(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig::plot_type

#endif  // PF_IMGUI_ELEMENTS_PLOTS_TYPES_DIGITAL_H
