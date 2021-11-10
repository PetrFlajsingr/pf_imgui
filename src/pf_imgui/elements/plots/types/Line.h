/**
 * @file Line.h
 * @brief Line graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_LINE_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_LINE_H

#include <implot.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>

namespace pf::ui::ig::plot_type {

/**
 * @brief 2D plot where each sample is connected to their neighbor via shortest line.
 */
class PF_IMGUI_EXPORT Line : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  /**
   * Construct Line.
   * @param elementName ID of the plot
   * @param caption text rendered above the plot
   */
  Line(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};
}// namespace pf::ui::ig::plot_type

#endif//PF_IMGUI_ELEMENTS_PLOTS_TYPES_LINE_H
