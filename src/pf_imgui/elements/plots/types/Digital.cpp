//
// Created by petr on 11/8/20.
//

#include "Digital.h"

namespace pf::ui::ig::plot_type {

Digital::Digital(const std::string &elementName, std::unique_ptr<Resource<std::string>> label) : LabeledPlotData(elementName, std::move(label)) {}

void Digital::renderImpl() {
  ImPlot::PlotDigital(getLabel().get().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size()));
}

}  // namespace pf::ui::ig::plot_type
