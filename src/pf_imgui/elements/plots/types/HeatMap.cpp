//
// Created by petr on 11/8/20.
//

#include "HeatMap.h"

namespace pf::ui::ig::plot_type {

HeatMap::HeatMap(const std::string &elementName, const std::string &caption, double minVal, double maxVal)
    : Element(elementName), LabeledPlotData(elementName, caption), min(minVal), max(maxVal) {}

void HeatMap::renderImpl() { ImPlot::PlotHeatmap(getLabel().c_str(), data.data(), rowSize, recordCount, min, max); }
}// namespace pf::ui::ig::plot_type