//
// Created by petr on 11/1/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_PLOT_H
#define PF_IMGUI_IMGUI_ELEMENTS_PLOT_H

#include <optional>
#include <pf_imgui/_export.h>
#include <src/pf_imgui/elements/interface/LabeledElement.h>
#include <src/pf_imgui/elements/interface/ResizableElement.h>
#include <vector>

namespace pf::ui::ig {

// TODO: advanced plot using implot

class PF_IMGUI_EXPORT SimplePlot : public LabeledElement, public ResizableElement {
 public:
  SimplePlot(const std::string &elementName, const std::string &caption, PlotType plotType,
             std::vector<float> values = {}, std::optional<std::string> overlayText = std::nullopt,
             const std::optional<std::size_t> &historyLimit = std::nullopt,
             float scaleLow = FLT_MAX, float scaleHigh = FLT_MAX, ImVec2 size = {0, 0});
  void addValue(float value);
  void clear();
  void setValues(const std::vector<float> &vals);

 protected:
  void renderImpl() override;

 private:
  PlotType plotType;
  std::vector<float> values;
  float scaleMin;
  float scaleMax;
  std::optional<std::string> overlayText = std::nullopt;
  std::optional<std::size_t> historyLimit = std::nullopt;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_PLOT_H
