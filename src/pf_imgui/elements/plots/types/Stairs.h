//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_STAIRS_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_STAIRS_H

#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <implot.h>
#include <string>

namespace pf::ui::ig::plot_type {

class PF_IMGUI_EXPORT Stairs : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  Stairs(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig::plot_type

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_STAIRS_H
