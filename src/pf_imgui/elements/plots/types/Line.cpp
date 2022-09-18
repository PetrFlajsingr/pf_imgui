//
// Created by petr on 11/8/20.
//

#include "Line.h"
#include <implot.h>

namespace pf::ui::ig::plot_type {

Line::Line(std::string_view elementName, std::string_view caption) : LabeledPlotData(elementName, caption) {}

void Line::renderImpl() {
  ImPlot::PlotLine(label->get().c_str(), xData.data(), yData.data(), static_cast<int>(xData.size()));
}

}  // namespace pf::ui::ig::plot_type
