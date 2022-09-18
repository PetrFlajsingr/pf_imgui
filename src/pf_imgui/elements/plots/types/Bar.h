/**
 * @file Bar.h
 * @brief Bar graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H

#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>

namespace pf::ui::ig::plot_type {

namespace details {
void renderPlotBarsVertical(const char *label, const double *xData, const double *yData, std::size_t size,
                            double barSize);
void renderPlotBarsHorizontal(const char *label, const double *xData, const double *yData, std::size_t size,
                              double barSize);
}  // namespace details

/**
 * @brief 2D bar plot, which can either be horizontal or vertical.
 * @see PlotType
 */
template<BarType Type = BarType::Vertical>
class PF_IMGUI_EXPORT Bar : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  /**
   * Construct Bar.
   * @param elementName ID of the plot
   * @param caption text rendered above the plot
   */
  Bar(std::string_view elementName, std::string_view caption) : LabeledPlotData(elementName, caption) {}

 protected:
  void renderImpl() override {
    if constexpr (Type == BarType::Vertical) {
      details::renderPlotBarsVertical(label->get().c_str(), xData.data(), yData.data(), xData.size(), 0.67);
    } else {
      details::renderPlotBarsHorizontal(label->get().c_str(), xData.data(), yData.data(), xData.size(), 0.67);
    }
  }
};
}  // namespace pf::ui::ig::plot_type

#endif  // PF_IMGUI_ELEMENTS_PLOTS_TYPES_BAR_H
