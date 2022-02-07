//
// Created by petr on 11/8/20.
//

#include "Stairs.h"

namespace pf::ui::ig::plot_type {

Stairs::Stairs(const std::string &elementName, std::unique_ptr<Resource<std::string>> label) : LabeledPlotData(elementName, std::move(label)) {}

void Stairs::renderImpl() {
  ImPlot::PlotStairs(getLabel().get().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size()));
}

}  // namespace pf::ui::ig::plot_type
