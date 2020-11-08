//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PLOT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PLOT_H

#include "../interface/LabeledElement.h"
#include "../interface/ResizableElement.h"
#include "types/PlotDataBase.h"
#include <bits/ranges_algo.h>
#include <optional>
#include <pf_imgui/_export.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

// TODO: pie chart
class PF_IMGUI_EXPORT Plot : public LabeledElement, public ResizableElement {
 public:
  Plot(const std::string &elementName, const std::string &caption,
       std::optional<std::string> xLabel = std::nullopt,
       std::optional<std::string> yLabel = std::nullopt, const ImVec2 &size = ImVec2{-1, 0});

 protected:
  void renderImpl() override;

 private:
  std::optional<std::string> xLabel = std::nullopt;
  std::optional<std::string> yLabel = std::nullopt;

  std::vector<std::unique_ptr<plot_type::PlotData>> datas;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PLOT_H
