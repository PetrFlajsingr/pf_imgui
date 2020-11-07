//
// Created by petr on 11/1/20.
//

#include "SimplePlot.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

SimplePlot::SimplePlot(const std::string &elementName, const std::string &caption, PlotType plotType,
           std::vector<float> values, std::optional<std::string> overlayText,
           const std::optional<std::size_t> &historyLimit)
    : Element(elementName), LabeledElement(elementName, caption), plotType(plotType),
      values(std::move(values)), overlayText(std::move(overlayText)), historyLimit(historyLimit) {}

void SimplePlot::renderImpl() {
  switch (plotType) {
    case PlotType::Lines:
      ImGui::PlotLines(getLabel().c_str(), values.data(), values.size(), 0,
                       overlayText.has_value() ? overlayText->c_str() : nullptr);
      break;
    case PlotType::Histogram:
      ImGui::PlotHistogram(getLabel().c_str(), values.data(), values.size(), 0,
                           overlayText.has_value() ? overlayText->c_str() : nullptr);
      break;
  }
}
void SimplePlot::addValue(float value) {
  values.emplace_back(value);
  if (historyLimit.has_value() && *historyLimit < values.size()) { values.erase(values.begin()); }
}
void SimplePlot::clear() { values.clear(); }
void SimplePlot::setValues(const std::vector<float> &vals) { values = vals; }

}// namespace pf::ui::ig