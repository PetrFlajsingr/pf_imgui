//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_PLOTDATABASE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_PLOTDATABASE_H

#include "../../interface/LabeledElement.h"
#include <concepts>
#include <pf_imgui/_export.h>
#include <range/v3/algorithm/minmax.hpp>
#include <range/v3/core.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig::plot_type {
template<typename T>
concept Plottable = std::convertible_to<T, double>;

template<Plottable T>
struct XYPlotData {
  T x;
  T y;
};

template<Plottable T>
struct XYErrorPlotData : XYPlotData<T> {
  T error;
};

enum class BarType { Horizontal, Vertical };

namespace details {
class DefaultPlotDataSetting {
 public:
  template<Plottable T>
  void setData(const std::vector<XYPlotData<T>> &newData) {
    xData = newData | ranges::views::transform([](const auto &val) { return static_cast<double>(val.x); })
        | ranges::to_vector;
    const auto [min, max] = ranges::minmax(xData);
    width = max - min;
    yData = newData | ranges::views::transform([](const auto &val) { return static_cast<double>(val.y); })
        | ranges::to_vector;
  }

 protected:
  std::vector<double> xData;
  std::vector<double> yData;
  double width;
};
}// namespace details

class PF_IMGUI_EXPORT PlotData : public virtual Element {
 public:
  explicit PlotData(const std::string &elementName);
};

class PF_IMGUI_EXPORT LabeledPlotData : public PlotData, public LabeledElement {
 public:
  LabeledPlotData(const std::string &elementName, const std::string &caption);
};

}// namespace pf::ui::ig::plot_type

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_PLOTDATABASE_H
