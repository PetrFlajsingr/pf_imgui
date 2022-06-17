//
// Created by petr on 11/7/20.
//

#include "FlameGraph.h"
#include <utility>

namespace pf::ui::ig {

FlameGraph::FlameGraph(FlameGraph::Config &&config)
    : ElementWithID(std::string{config.name.value}), Labellable(std::string{config.label.value}),
      Resizable(config.size), overlay(std::move(config.overlay)) {}

FlameGraph::FlameGraph(const std::string &elementName, const std::string &label, const Size &size,
                       std::optional<std::string> overlay)
    : ElementWithID(elementName), Labellable(label), Resizable(size), overlay(std::move(overlay)) {}

void FlameGraph::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  ImGuiWidgetFlameGraph::PlotFlame(getLabel().c_str(), samples, overlay, FLT_MAX, FLT_MAX,
                                   static_cast<ImVec2>(getSize()));
}

void FlameGraph::setOverlay(std::string text) { overlay = std::move(text); }

void FlameGraph::disableOverlay() { overlay = std::nullopt; }

void FlameGraph::setSamples(const std::vector<ImGuiWidgetFlameGraph::FlameGraphSample> &newSamples) {
  samples = newSamples;
}

}  // namespace pf::ui::ig
