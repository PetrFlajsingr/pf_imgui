//
// Created by petr on 11/8/20.
//

#include "Stairs.h"
#include <implot.h>

namespace pf::ui::ig::plot_type {

Stairs::Stairs(std::string_view elementName, std::string_view caption) : LabeledPlotData(elementName, caption) {}

void Stairs::renderImpl() { ImPlot::PlotStairs(label->get().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size())); }

}  // namespace pf::ui::ig::plot_type
