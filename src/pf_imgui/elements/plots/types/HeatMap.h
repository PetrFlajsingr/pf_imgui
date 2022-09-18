/**
 * @file HeatMap.h
 * @brief HeatMap graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_HEATMAP_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_HEATMAP_H

#include <algorithm>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig::plot_type {

/**
 * @brief 2D plot where data is visualised as a heat map.
 *
 * minVal and maxVal define where the 'coldest' and the 'hottest' colors are
 */
class PF_IMGUI_EXPORT HeatMap : public LabeledPlotData {
 public:
  /**
   * Construct HeatMap.
   * @param elementName ID of the element
   * @param caption text rendered next to data
   * @param minVal 'coldest' value
   * @param maxVal 'hottest' vlaue
   */
  HeatMap(std::string_view elementName, std::string_view caption, double minVal = 0.0, double maxVal = 0.0);

  /**
  * Set new plot data.
  * @param newData new data
  * @tparam type of data to plot
  */
  template<Plottable T>
  void setData(const std::vector<std::vector<T>> &newData) {
    if (newData.empty()) {
      data.clear();
      rowSize = 0;
      recordCount = 0;
      return;
    }
    recordCount = newData.size();
    rowSize = newData[0].size();
    std::ranges::for_each(newData, [this](const auto &row) { assert(row.size() == rowSize); });
    data = newData | ranges::views::join
        | ranges::views::transform([](const auto &val) { return static_cast<double>(val); }) | ranges::to_vector;
  }

 protected:
  void renderImpl() override;

 private:
  std::vector<double> data;
  std::size_t rowSize = 0;
  std::size_t recordCount = 0;
  double min;
  double max;
};
}  // namespace pf::ui::ig::plot_type

#endif  // PF_IMGUI_ELEMENTS_PLOTS_TYPES_HEATMAP_H
