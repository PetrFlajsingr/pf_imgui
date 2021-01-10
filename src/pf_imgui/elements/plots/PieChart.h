//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PIECHART_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PIECHART_H

#include "../interface/LabeledElement.h"
#include "../interface/ResizableElement.h"
#include "types/PlotDataBase.h"
#include <pf_imgui/_export.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

template<plot_type::Plottable T>
struct PieChartSample {
  std::string label;
  T value;
};

class PieChart : public LabeledElement, public ResizableElement {
 public:
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
