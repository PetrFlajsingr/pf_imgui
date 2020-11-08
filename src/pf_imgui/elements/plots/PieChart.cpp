//
// Created by petr on 11/8/20.
//

#include "PieChart.h"
#include <implot.h>

namespace pf::ui::ig {

void PieChart::renderImpl() {
  if (dataChanged) {
    dataChanged = false;
    labelsCstr = data
        | ranges::views::transform([](const auto &pair) { return pair.first.c_str(); })
        | ranges::to_vector;
    values = data | ranges::views::transform([](const auto &pair) { return pair.second; })
        | ranges::to_vector;
  }
  if (ImPlot::BeginPlot(getLabel().c_str(), nullptr, nullptr, getSize())) {
    ImPlot::PlotPieChart(labelsCstr.data(), values.data(), values.size(), 0.5, 0.5, 0.4, true);
    ImPlot::EndPlot();
  }
}
}// namespace pf::ui::ig