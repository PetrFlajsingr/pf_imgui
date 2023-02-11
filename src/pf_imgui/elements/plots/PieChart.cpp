//
// Created by petr on 11/8/20.
//

#include "PieChart.h"
#include <implot.h>
#include <pf_common/ScopeExit.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

PieChart::PieChart(PieChart::Config &&config) : PieChart(config.name, config.label, config.size) {}

PieChart::PieChart(std::string_view elementName, std::string_view labelText, Size initialSize)
    : ElementWithID(elementName), label(std::string{labelText}), size(initialSize) {}

void PieChart::renderImpl() {
  if (dataChanged) {
    dataChanged = false;
    labelsCstr = data | ranges::views::transform([](const auto &pair) { return pair.first.c_str(); }) | ranges::to_vector;
    values = data | ranges::views::transform([](const auto &pair) { return pair.second; }) | ranges::to_vector;
  }

  if (ImPlot::BeginPlot(label->get().c_str(), static_cast<ImVec2>(*size))) {
    ScopeExit endPlot{[] { ImPlot::EndPlot(); }};
    ImPlot::PlotPieChart(labelsCstr.data(), values.data(), static_cast<int>(values.size()), 0.5, 0.5, 0.4);
  }
}

}  // namespace pf::ui::ig