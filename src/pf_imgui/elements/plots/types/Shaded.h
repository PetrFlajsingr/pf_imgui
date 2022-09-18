/**
 * @file Shaded.h
 * @brief Shaded graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_SHADED_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_SHADED_H

#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>

namespace pf::ui::ig::plot_type {

/**
 * @brief 2D plot where and area beneath the data set is colored.
 */
class PF_IMGUI_EXPORT Shaded : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  /**
   * Construct Shaded.
   * @param elementName ID of the plot
   * @param caption text rendered above the plot
   */
  Shaded(std::string_view elementName, std::string_view caption);

 protected:
  void renderImpl() override;
};
}  // namespace pf::ui::ig::plot_type

#endif  // PF_IMGUI_ELEMENTS_PLOTS_TYPES_SHADED_H
