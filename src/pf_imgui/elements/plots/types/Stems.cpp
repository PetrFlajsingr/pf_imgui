//
// Created by petr on 11/8/20.
//

#include "Stems.h"

namespace pf::ui::ig::plot_type {

Stems::Stems(const std::string &elementName, const std::string &caption) : LabeledPlotData(elementName, caption) {}

void Stems::renderImpl() {
  ImPlot::PlotStems(getLabel().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size()));
}

}  // namespace pf::ui::ig::plot_type
