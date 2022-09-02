//
// Created by petr on 11/8/20.
//

#include "PieChart.h"
#include <implot.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

PieChart::PieChart(PieChart::Config &&config)
    : ElementWithID(std::string{config.name.value}), Resizable(config.size), label(std::string{config.label.value}) {}

PieChart::PieChart(const std::string &elementName, const std::string &labelText, const Size &initialSize)
    : ElementWithID(elementName), Resizable(initialSize), label(labelText) {}

void PieChart::renderImpl() {
  if (dataChanged) {
    dataChanged = false;
    labelsCstr =
        data | ranges::views::transform([](const auto &pair) { return pair.first.c_str(); }) | ranges::to_vector;
    values = data | ranges::views::transform([](const auto &pair) { return pair.second; }) | ranges::to_vector;
  }

  if (ImPlot::BeginPlot(label.get().c_str(), static_cast<ImVec2>(getSize()))) {
    RAII endPlot{[] { ImPlot::EndPlot(); }};
    ImPlot::PlotPieChart(labelsCstr.data(), values.data(), static_cast<int>(values.size()), 0.5, 0.5, 0.4);
  }
}

}  // namespace pf::ui::ig