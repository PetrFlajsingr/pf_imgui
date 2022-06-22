//
// Created by petr on 11/8/20.
//

#include "PieChart.h"
#include <implot.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

PieChart::PieChart(PieChart::Config &&config)
    : ElementWithID(std::string{config.name.value}), Labellable(std::string{config.label.value}), Resizable(config.size) {}

PieChart::PieChart(const std::string &name, const std::string &label, const Size &size)
    : ElementWithID(name), Labellable(label), Resizable(size) {}

void PieChart::renderImpl() {
  if (dataChanged) {
    dataChanged = false;
    labelsCstr =
        data | ranges::views::transform([](const auto &pair) { return pair.first.c_str(); }) | ranges::to_vector;
    values = data | ranges::views::transform([](const auto &pair) { return pair.second; }) | ranges::to_vector;
  }

  if (ImPlot::BeginPlot(getLabel().c_str(), static_cast<ImVec2>(getSize()))) {
    RAII endPlot{[] { ImPlot::EndPlot(); }};
    ImPlot::PlotPieChart(labelsCstr.data(), values.data(), static_cast<int>(values.size()), 0.5, 0.5, 0.4);
  }
}

}  // namespace pf::ui::ig