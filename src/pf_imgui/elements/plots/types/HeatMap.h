//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_HEATMAP_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_HEATMAP_H

#include "PlotDataBase.h"
#include <bits/ranges_algo.h>
#include <implot.h>

namespace pf::ui::ig::plot_type {

class PF_IMGUI_EXPORT HeatMap : public LabeledPlotData {
 public:
  HeatMap(const std::string &elementName, const std::string &caption, double minVal = 0.0,
          double maxVal = 0.0);

  template<Plottable T>
  void setData(const std::vector<std::vector<T>> &newData) {
    if (newData.empty()) {
      data.clear();
      rowSize = 0;
      recordCount = 0;
      return;
    }
    rowSize = newData[0].size();
    std::ranges::for_each(newData, [this](const auto &row) { assert(row.size() == rowSize); });
    data = newData | ranges::views::join
        | ranges::views::transform([](const auto &val) { return static_cast<double>(val); })
        | ranges::to_vector;
  }

 protected:
  void renderImpl() override;

 private:
  std::vector<double> data;
  std::size_t rowSize = 0;
  std::size_t recordCount = 0;
  double min;
  double max;
};
}// namespace pf::ui::ig::plot_type

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_HEATMAP_H
