//
// Created by petr on 11/8/20.
//

#include "Shaded.h"

namespace pf::ui::ig::plot_type {

Shaded::Shaded(const std::string &elementName, const std::string &caption) : LabeledPlotData(elementName, caption) {}

void Shaded::renderImpl() { ImPlot::PlotShaded(getLabel().c_str(), xData.data(), yData.data(), xData.size()); }

}// namespace pf::ui::ig::plot_type