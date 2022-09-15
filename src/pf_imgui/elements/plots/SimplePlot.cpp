//
// Created by petr on 11/1/20.
//

#include "SimplePlot.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

SimplePlot::SimplePlot(SimplePlot::Config &&config)
    : SimplePlot(config.name, config.label, config.type, config.values, config.overlay, config.maxHistoryCount,
                 config.scaleLow, config.scaleHigh, config.size) {}

SimplePlot::SimplePlot(std::string_view elementName, std::string_view labelText, PlotType type,
                       std::vector<float> plotValues, std::optional<std::string> plotOverlayText,
                       const std::optional<std::size_t> &maxHistorySize, float scaleLow, float scaleHigh,
                       Size initialSize)
    : ElementWithID(elementName), label(std::string{labelText}), size(initialSize), plotType(type),
      values(std::move(plotValues)), scaleMin(scaleLow), scaleMax(scaleHigh), overlayText(std::move(plotOverlayText)),
      historyLimit(maxHistorySize) {}

void SimplePlot::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  switch (plotType) {
    case PlotType::Lines:
      ImGui::PlotLines(label->get().c_str(), values.data(), static_cast<int>(values.size()), 0,
                       overlayText.has_value() ? overlayText->c_str() : nullptr, scaleMin, scaleMax,
                       static_cast<ImVec2>(*size));
      break;
    case PlotType::Histogram:
      ImGui::PlotHistogram(label->get().c_str(), values.data(), static_cast<int>(values.size()), 0,
                           overlayText.has_value() ? overlayText->c_str() : nullptr, scaleMin, scaleMax,
                           static_cast<ImVec2>(*size));
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
