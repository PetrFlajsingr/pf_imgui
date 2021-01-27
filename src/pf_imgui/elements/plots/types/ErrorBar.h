//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_ERRORBAR_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_ERRORBAR_H

#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <algorithm>
#include <implot.h>
#include <string>
#include <vector>

namespace pf::ui::ig::plot_type {

template<BarType Type = BarType::Horizontal>
class PF_IMGUI_EXPORT ErrorBar : public LabeledPlotData, details::DefaultPlotDataSetting {
 public:
  ErrorBar(const std::string &elementName, const std::string &caption)
      : Element(elementName), LabeledPlotData(elementName, caption) {}

  template<Plottable T>
  void setData(const std::vector<XYErrorPlotData<T>> &newData) {
    const auto xyData = newData | ranges::views::transform([](const auto &data) { return XYPlotData(data.x, data.y); })
        | ranges::to_vector;
    details::DefaultPlotDataSetting::setData(xyData);
    error = newData | ranges::views::transform([](const auto &data) { return static_cast<double>(data.error); })
        | ranges::to_vector;
  }

 protected:
  void renderImpl() override {
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
