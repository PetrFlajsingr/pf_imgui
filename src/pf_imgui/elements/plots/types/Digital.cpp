//
// Created by petr on 11/8/20.
//

#include "Digital.h"

namespace pf::ui::ig::plot_type {

Digital::Digital(const std::string &elementName, const std::string &caption)
    : Element(elementName), LabeledPlotData(elementName, caption) {}

void Digital::renderImpl() { ImPlot::PlotDigital(getLabel().c_str(), xData.data(), yData.data(), xData.size()); }
}// namespace pf::ui::ig::plot_type