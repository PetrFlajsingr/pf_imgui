//
// Created by petr on 11/7/20.
//

#include "FlameGraph.h"

#include <utility>

namespace pf::ui::ig {

FlameGraph::FlameGraph(const std::string &elementName, const std::string &label, const ImVec2 &size,
                       std::optional<std::string> overlay)
    : Element(elementName), Labellable(label), Resizable(size),
      overlay(std::move(overlay)) {}

void FlameGraph::renderImpl() {
  ImGuiWidgetFlameGraph::PlotFlame(getLabel().c_str(), samples, overlay, FLT_MAX, FLT_MAX, getSize());
}

void FlameGraph::setOverlay(std::string text) { overlay = std::move(text); }

void FlameGraph::disableOverlay() { overlay = std::nullopt; }

void FlameGraph::setSamples(const std::vector<ImGuiWidgetFlameGraph::FlameGraphSample> &newSamples) {
  samples = newSamples;
}

}// namespace pf::ui::ig
