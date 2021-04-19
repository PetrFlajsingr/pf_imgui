/**
 * @file ErrorBar.h
 * @brief ErrorBar graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_ERRORBAR_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_ERRORBAR_H

#include <algorithm>
#include <implot.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>
#include <vector>

namespace pf::ui::ig::plot_type {

/**
 * @brief 2D bar plot, which can either be horizontal or vertical. It shows error margin for each bar.
 * @see PlotType
 * @see XYErrorPlotData
 */
template<BarType Type = BarType::Horizontal>
class PF_IMGUI_EXPORT ErrorBar : public LabeledPlotData, details::DefaultPlotDataSetting {
 public:
  /**
   * Construct ErrorBar.
   * @param elementName ID of the plot
   * @param caption text rendered above the plot
   */
  ErrorBar(const std::string &elementName, const std::string &caption)
      : LabeledPlotData(elementName, caption) {}

  /**
   * Set new plot data.
   * @param newData new data
   * @tparam type of data to plot
   */
  template<Plottable T>
  void setData(const std::ranges::range auto &newData) requires(
      std::same_as<std::ranges::range_value_t<decltype(newData)>, XYErrorPlotData<T>>) {
    const auto xyData = newData | ranges::views::transform([](const auto &data) { return XYPlotData(data.x, data.y); })
        | ranges::to_vector;
    details::DefaultPlotDataSetting::setData(xyData);
    error = newData | ranges::views::transform([](const auto &data) { return static_cast<double>(data.error); })
        | ranges::to_vector;
  }

 protected : void renderImpl() override {
    if constexpr (Type == BarType::Vertical) {
      ImPlot::PlotErrorBars(getLabel().c_str(), xData.data(), yData.data(), error.data(), xData.size());
    } else {
      ImPlot::PlotErrorBarsH(getLabel().c_str(), xData.data(), yData.data(), error.data(), xData.size());
    }
  }

 private:
  std::vector<double> error;
};
}// namespace pf::ui::ig::plot_type

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_ERRORBAR_H
