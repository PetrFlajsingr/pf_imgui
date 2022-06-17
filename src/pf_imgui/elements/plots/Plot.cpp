//
// Created by petr on 11/8/20.
//

#include "Plot.h"
#include <algorithm>
#include <implot.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Plot::Plot(Plot::Config &&config)
    : ElementWithID(std::string{config.name.value}), Labellable(std::string{config.label.value}), Resizable(config.size),
      xLabel(std::move(config.xLabel)), yLabel(std::move(config.yLabel)) {}

Plot::Plot(const std::string &elementName, const std::string &label, std::optional<std::string> xLabel,
           std::optional<std::string> yLabel, const Size &size)
    : ElementWithID(elementName), Labellable(label), Resizable(size), xLabel(std::move(xLabel)), yLabel(std::move(yLabel)) {}

// TODO: flags
void Plot::renderImpl() {
  if (ImPlot::BeginPlot(getLabel().c_str(), static_cast<ImVec2>(getSize()))) {
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
