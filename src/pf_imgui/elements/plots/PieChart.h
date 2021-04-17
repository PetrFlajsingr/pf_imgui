//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_ELEMENTS_PLOTS_PIECHART_H
#define PF_IMGUI_ELEMENTS_PLOTS_PIECHART_H

#include <pf_imgui/_export.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Data storage for PieChart samples.
 */
template<plot_type::Plottable T>
struct PieChartSample {
  std::string label; /**< Text to be rendered next to the data */
  T value; /**< Sample value */
};

/**
 * @brief Typical pie chart.
 *
 * Each sample shows as a percentage in the sum of all sample values.
 */
class PieChart : public Element, public Labellable, public Resizable {
 public:
  /**
   * Construct PieChart.
   * @param name ID of the chart
   * @param label text rendered above the chart
   * @param size size on display
   */
  PieChart(const std::string &name, const std::string &label, const ImVec2 &size);
  /**
   * Add a new sample to the graph.
   * @param sample sample to be added
   */
  template<plot_type::Plottable T>
  void addValue(PieChartSample<T> &&sample) {
    data.emplace_back(sample.label, static_cast<double>(sample.value));
    dataChanged = true;
  }

 protected:
  void renderImpl() override;

 private:
  std::vector<std::pair<std::string, double>> data;
  std::vector<const char *> labelsCstr;
  std::vector<double> values;
  bool dataChanged = false;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PIECHART_H
