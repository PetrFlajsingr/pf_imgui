//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H

#include <implot.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>

namespace pf::ui::ig::plot_type {

template<BarType Type = BarType::Vertical>
class PF_IMGUI_EXPORT Bar : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  Bar(const std::string &elementName, const std::string &caption)
      : Element(elementName), LabeledPlotData(elementName, caption) {}

 protected:
  void renderImpl() override {
    if constexpr (Type == BarType::Vertical) {
      ImPlot::PlotBars(getLabel().c_str(), xData.data(), yData.data(), xData.size(), 0.67);
    } else {
      ImPlot::PlotBarsH(getLabel().c_str(), xData.data(), yData.data(), xData.size(), 0.67);
    }
  }
};
}// namespace pf::ui::ig::plot_type

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H
