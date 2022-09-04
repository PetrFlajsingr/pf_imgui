//
// Created by petr on 11/8/20.
//

#include "Plot.h"
#include <algorithm>
#include <implot.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Plot::Plot(Plot::Config &&config)
    : ElementWithID(std::string{config.name.value}), size(config.size), label(std::string{config.label.value}),
      xLabel(std::move(config.xLabel)), yLabel(std::move(config.yLabel)) {}

Plot::Plot(const std::string &elementName, const std::string &labelText, std::optional<std::string> xLabelText,
           std::optional<std::string> yLabelText, const Size &initialSize)
    : ElementWithID(elementName), size(initialSize), label(labelText), xLabel(std::move(xLabelText)),
      yLabel(std::move(yLabelText)) {}

// TODO: flags
void Plot::renderImpl() {
  if (ImPlot::BeginPlot(label->get().c_str(), static_cast<ImVec2>(*size))) {
    RAII endPopup{[] { ImPlot::EndPlot(); }};
    ImPlot::SetupAxis(ImAxis_X1, xLabel.has_value() ? xLabel->c_str() : nullptr);
    ImPlot::SetupAxis(ImAxis_Y1, yLabel.has_value() ? yLabel->c_str() : nullptr);
    std::ranges::for_each(datas, [](auto &data) { data->render(); });
  }
}

void Plot::removeData(const std::string &dataName) {
  if (const auto iter =
          std::ranges::find_if(datas, [dataName](const auto &data) { return data->getName() == dataName; });
      iter != datas.end()) {
    datas.erase(iter);
  }
}

}  // namespace pf::ui::ig
