//
// Created by petr on 11/1/20.
//

#include "SimplePlot.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

SimplePlot::SimplePlot(SimplePlot::Config &&config)
    : ElementWithID(std::string{config.name.value}), Labellable(std::string{config.label.value}),
      Resizable(config.size), plotType(config.type), values(std::move(config.values)), scaleMin(config.scaleLow),
      scaleMax(config.scaleHigh), overlayText(std::move(config.overlay)), historyLimit(config.maxHistoryCount) {}

SimplePlot::SimplePlot(const std::string &elementName, const std::string &label, PlotType plotType,
                       std::vector<float> values, std::optional<std::string> overlayText,
                       const std::optional<std::size_t> &historyLimit, float scaleLow, float scaleHigh, Size size)
    : ElementWithID(elementName), Labellable(label), Resizable(size), plotType(plotType), values(std::move(values)),
      scaleMin(scaleLow), scaleMax(scaleHigh), overlayText(std::move(overlayText)), historyLimit(historyLimit) {}

void SimplePlot::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  switch (plotType) {
    case PlotType::Lines:
      ImGui::PlotLines(getLabel().c_str(), values.data(), static_cast<int>(values.size()), 0,
                       overlayText.has_value() ? overlayText->c_str() : nullptr, scaleMin, scaleMax,
                       static_cast<ImVec2>(getSize()));
      break;
    case PlotType::Histogram:
      ImGui::PlotHistogram(getLabel().c_str(), values.data(), static_cast<int>(values.size()), 0,
                           overlayText.has_value() ? overlayText->c_str() : nullptr, scaleMin, scaleMax,
                           static_cast<ImVec2>(getSize()));
      break;
  }
}

void SimplePlot::addValue(float value) {
  values.emplace_back(value);
  if (historyLimit.has_value() && *historyLimit < values.size()) { values.erase(values.begin()); }
}

void SimplePlot::clear() { values.clear(); }

void SimplePlot::setValues(const std::vector<float> &vals) { values = vals; }

}  // namespace pf::ui::ig
