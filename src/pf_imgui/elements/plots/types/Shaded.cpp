//
// Created by petr on 11/8/20.
//

#include "Shaded.h"

namespace pf::ui::ig::plot_type {

Shaded::Shaded(const std::string &elementName, std::unique_ptr<Resource<std::string>> label) : LabeledPlotData(elementName, std::move(label)) {}

void Shaded::renderImpl() {
  ImPlot::PlotShaded(getLabel().get().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size()));
}

}  // namespace pf::ui::ig::plot_type
