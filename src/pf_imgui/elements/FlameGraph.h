//
// Created by petr on 11/7/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_FLAMEGRAPH_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_FLAMEGRAPH_H

#include "interface/LabeledElement.h"
#include <optional>
#include <src/pf_imgui/elements/interface/ResizableElement.h>
#include <include/ImGUiFlamegraph/imgui_widget_flamegraph.h>

namespace pf::ui::ig {

class FlameGraph : public LabeledElement, public ResizableElement {
 public:
  FlameGraph(const std::string &elementName, const std::string &caption,
             const ImVec2 &size = ImVec2{0, 0}, std::optional<std::string> overlay = std::nullopt);

  void setOverlay(std::string text);
  void disableOverlay();

 protected:
  void renderImpl() override;

 private:
  std::optional<std::string> overlay;
  std::vector<ImGuiWidgetFlameGraph::FlameGraphSample> samples;
};

}

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_FLAMEGRAPH_H
