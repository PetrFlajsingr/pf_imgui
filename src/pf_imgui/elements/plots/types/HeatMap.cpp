//
// Created by petr on 11/8/20.
//

#include "HeatMap.h"

namespace pf::ui::ig::plot_type {

HeatMap::HeatMap(const std::string &elementName, std::unique_ptr<Resource<std::string>> label, double minVal, double maxVal)
    : LabeledPlotData(elementName, std::move(label)), min(minVal), max(maxVal) {}

void HeatMap::renderImpl() {
  ImPlot::PlotHeatmap(getLabel().get().c_str(), data.data(), static_cast<int>(rowSize), static_cast<int>(recordCount), min,
                      max);
}

}  // namespace pf::ui::ig::plot_type
