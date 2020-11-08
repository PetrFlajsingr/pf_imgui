//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_STEMS_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_STEMS_H

#include "PlotDataBase.h"
#include <implot.h>

namespace pf::ui::ig::plot_type {

class PF_IMGUI_EXPORT Stems : public LabeledPlotData, public details::DefaultPlotDataSetting {
 public:
  Stems(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};
}// namespace pf::ui::ig::plot_type

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_TYPES_STEMS_H
