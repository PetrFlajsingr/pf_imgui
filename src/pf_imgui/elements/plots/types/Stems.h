/**
 * @file Stems.h
 * @brief Stem graph data.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_TYPES_STEMS_H
#define PF_IMGUI_ELEMENTS_PLOTS_TYPES_STEMS_H

#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <string>

namespace pf::ui::ig::plot_type {

/**
 * @brief 2D graph, where each value is rendered as a stem on X axis.
 *
 * Example:
 *        o
 *        |
 * o      |
 * |  o   |
 * |  |   |
 * ________
*/
class PF_IMGUI_EXPORT Stems : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  /**
   * Construct Stems.
   * @param elementName ID of the plot
   * @param caption text rendered above the plot
   */
  Stems(std::string_view elementName, std::string_view caption);

 protected:
  void renderImpl() override;
};
}  // namespace pf::ui::ig::plot_type

#endif  // PF_IMGUI_ELEMENTS_PLOTS_TYPES_STEMS_H
