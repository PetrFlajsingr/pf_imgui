//
// Created by petr on 11/8/20.
//

#include "Line.h"

namespace pf::ui::ig::plot_type {

Line::Line(const std::string &elementName, std::unique_ptr<Resource<std::string>> label) : LabeledPlotData(elementName, std::move(label)) {}

void Line::renderImpl() {
  ImPlot::PlotLine(getLabel().get().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size()));
}

}  // namespace pf::ui::ig::plot_type
