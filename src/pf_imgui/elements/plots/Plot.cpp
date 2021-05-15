//
// Created by petr on 11/8/20.
//

#include "Plot.h"
#include <algorithm>
#include <implot.h>

namespace pf::ui::ig {

Plot::Plot(const std::string &elementName, const std::string &label, std::optional<std::string> xLabel,
           std::optional<std::string> yLabel, const Size &size)
    : Element(elementName), Labellable(label), Resizable(size), xLabel(std::move(xLabel)), yLabel(std::move(yLabel)) {}

void Plot::renderImpl() {
  if (ImPlot::BeginPlot(getLabel().c_str(), xLabel.has_value() ? xLabel->c_str() : nullptr,
                        yLabel.has_value() ? yLabel->c_str() : nullptr, getSize().asImVec())) {
    std::ranges::for_each(datas, [](auto &data) { data->render(); });
    ImPlot::EndPlot();
  }
}
void Plot::removeData(const std::string &name) {
  if (const auto iter = std::ranges::find_if(datas, [name](const auto &data) { return data->getName() == name; });
      iter != datas.end()) {
    datas.erase(iter);
  }
}
}// namespace pf::ui::ig