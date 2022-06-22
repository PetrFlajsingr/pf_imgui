/**
 * @file Bar.h
 * @brief Bar graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H

#include <implot.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>

namespace pf::ui::ig::plot_type {

/**
 * @brief 2D bar plot, which can either be horizontal or vertical.
 * @see PlotType
 */
template<BarType Type = BarType::Vertical>
class PF_IMGUI_EXPORT Bar : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  /**
   * Construct Bar.
   * @param elementName ID of the plot
   * @param caption text rendered above the plot
   */
  Bar(const std::string &elementName, const std::string &caption) : LabeledPlotData(elementName, caption) {}

 protected:
  void renderImpl() override {
    if constexpr (Type == BarType::Vertical) {
      ImPlot::PlotBars(getLabel().c_str(), xData.data(), yData.data(), xData.size(), 0.67, 0.);
    } else {
      ImPlot::PlotBars(getLabel().c_str(), xData.data(), yData.data(), xData.size(), 0.67, 0.,
                       ImPlotBarsFlags_Horizontal);
    }
  }
};
}  // namespace pf::ui::ig::plot_type

#endif  // PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H
