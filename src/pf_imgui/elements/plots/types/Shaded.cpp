//
// Created by petr on 11/8/20.
//

#include "Shaded.h"

namespace pf::ui::ig::plot_type {

Shaded::Shaded(std::string_view elementName, std::string_view caption) : LabeledPlotData(elementName, caption) {}

void Shaded::renderImpl() {
  ImPlot::PlotShaded(label->get().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size()));
}

}  // namespace pf::ui::ig::plot_type
