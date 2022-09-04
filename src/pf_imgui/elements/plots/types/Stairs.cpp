//
// Created by petr on 11/8/20.
//

#include "Stairs.h"

namespace pf::ui::ig::plot_type {

Stairs::Stairs(const std::string &elementName, const std::string &caption) : LabeledPlotData(elementName, caption) {}

void Stairs::renderImpl() {
  ImPlot::PlotStairs(label->get().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size()));
}

}  // namespace pf::ui::ig::plot_type
