//
// Created by petr on 11/8/20.
//

#include "Line.h"

namespace pf::ui::ig::plot_type {
Line::Line(const std::string &elementName, const std::string &caption)
    : Element(elementName), LabeledPlotData(elementName, caption) {}

void Line::renderImpl() {
  ImPlot::PlotLine(getLabel().c_str(), xData.data(), yData.data(), xData.size());
}

}// namespace pf::ui::ig::plot_type