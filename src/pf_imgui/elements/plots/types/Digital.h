/**
 * @file Digital.h
 * @brief Digital graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_DIGITAL_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_DIGITAL_H

#include <pf_imgui/elements/plots/types/PlotDataBase.h>

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
  Digital(std::string_view elementName, std::string_view caption);

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig::plot_type

#endif  // PF_IMGUI_ELEMENTS_PLOTS_TYPES_DIGITAL_H
