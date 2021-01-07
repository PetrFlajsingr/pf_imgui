//
// Created by petr on 11/7/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_FLAMEGRAPH_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_FLAMEGRAPH_H

#include "interface/LabeledElement.h"
#include <include/ImGuiFlamegraph/imgui_widget_flamegraph.h>
#include <optional>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/ResizableElement.h>

namespace pf::ui::ig {

using FlameGraphSample = ImGuiWidgetFlameGraph::FlameGraphSample;
class PF_IMGUI_EXPORT FlameGraph : public LabeledElement, public ResizableElement {
 public:
  FlameGraph(const std::string &elementName, const std::string &caption, const ImVec2 &size = ImVec2{0, 0},
             std::optional<std::string> overlay = std::nullopt);

  void setOverlay(std::string text);
  void disableOverlay();

  void setSamples(const std::vector<ImGuiWidgetFlameGraph::FlameGraphSample> &newSamples);

 protected:
  void renderImpl() override;

 private:
  std::optional<std::string> overlay;
  std::vector<ImGuiWidgetFlameGraph::FlameGraphSample> samples;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_FLAMEGRAPH_H
