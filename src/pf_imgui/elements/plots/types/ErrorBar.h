/**
 * @file ErrorBar.h
 * @brief ErrorBar graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_ERRORBAR_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_ERRORBAR_H

#include <algorithm>
#include <pf_common/concepts/ranges.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

namespace pf::ui::ig::plot_type {

namespace details {
void renderErrorBarVertical(const char *label, const double *xData, const double *yData, const double *errorData,
                            int size);
void renderErrorBarHorizontal(const char *label, const double *xData, const double *yData, const double *errorData,
                              int size);
}  // namespace details

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
  ErrorBar(std::string_view elementName, std::string_view caption) : LabeledPlotData(elementName, caption) {}

  /**
   * Set new plot data.
   * @param newData new data
   * @tparam type of data to plot
   */
  template<Plottable T>
  void setData(const RangeOf<XYErrorPlotData<T>> auto &newData) {
    const auto xyData = newData | ranges::views::transform([](const auto &data) { return XYPlotData(data.x, data.y); })
        | ranges::to_vector;
    details::DefaultPlotDataSetting::setData(xyData);
    error = newData | ranges::views::transform([](const auto &data) { return static_cast<double>(data.error); })
        | ranges::to_vector;
  }

 protected:
  void renderImpl() override {
    if constexpr (Type == BarType::Vertical) {
      details::renderErrorBarVertical(label->get().c_str(), xData.data(), yData.data(), error.data(), xData.size());
    } else {
      details::renderErrorBarHorizontal(label->get().c_str(), xData.data(), yData.data(), error.data(), xData.size());
    }
  }

 private:
  std::vector<double> error;
};
}  // namespace pf::ui::ig::plot_type

#endif  // PF_IMGUI_ELEMENTS_PLOTS_TYPES_ERRORBAR_H
