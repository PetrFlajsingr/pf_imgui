/**
 * @file PieChart.h
 * @brief PieChart element.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_PIECHART_H
#define PF_IMGUI_ELEMENTS_PLOTS_PIECHART_H

#include <pf_common/Explicit.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/reactive/Observable.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Data storage for PieChart samples.
 */
template<plot_type::Plottable T>
struct PF_IMGUI_EXPORT PieChartSample {
  std::string label; /**< Text to be rendered next to the data */
  T value;           /**< Sample value */
};

/**
 * @brief Typical pie chart.
 *
 * Each sample shows as a percentage in the sum of all sample values.
 */
class PF_IMGUI_EXPORT PieChart : public ElementWithID {
 public:
  /**
   * @brief Struct for construction of PieChart.
   */
  struct Config {
    using Parent = PieChart;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered above the chart */
    Explicit<Size> size;              /*!< Size on display */
  };
  /**
   * Construct PieChart
   * @param config construction arguments @see PieChart::Config
   */
  explicit PieChart(Config &&config);
  /**
   * Construct PieChart.
   * @param elementName ID of the chart
   * @param labelText text rendered above the chart
   * @param initialSize size on display
   */
  PieChart(const std::string &elementName, const std::string &labelText, const Size &initialSize);
  /**
   * Add a new sample to the graph.
   * @param sample sample to be added
   */
  template<plot_type::Plottable T>
  void addValue(PieChartSample<T> &&sample) {
    data.emplace_back(sample.label, static_cast<double>(sample.value));
    dataChanged = true;
  }

  Observable<Label> label;

  Observable<Size> size;

 protected:
  void renderImpl() override;

 private:
  std::vector<std::pair<std::string, double>> data;
  std::vector<const char *> labelsCstr;
  std::vector<double> values;
  bool dataChanged = false;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_PLOTS_PIECHART_H
