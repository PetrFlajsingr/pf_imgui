//
// Created by petr on 11/7/20.
//

#include "FlameGraph.h"
#include <utility>

namespace pf::ui::ig {

FlameGraph::FlameGraph(FlameGraph::Config &&config)
    : ElementWithID(config.name.value), label(std::string{config.label.value}), size(config.size),
      overlay(std::move(config.overlay)) {}

FlameGraph::FlameGraph(std::string_view elementName, std::string_view labelText, Size initialSize,
                       std::optional<std::string> graphOverlay)
    : ElementWithID(elementName), label(std::string{labelText}), size(initialSize), overlay(std::move(graphOverlay)) {}

void FlameGraph::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGuiWidgetFlameGraph::PlotFlame(label->get().c_str(), samples, overlay, FLT_MAX, FLT_MAX,
                                   static_cast<ImVec2>(*size));
}

void FlameGraph::setOverlay(std::string text) { overlay = std::move(text); }

void FlameGraph::disableOverlay() { overlay = std::nullopt; }

void FlameGraph::setSamples(const std::vector<ImGuiWidgetFlameGraph::FlameGraphSample> &newSamples) {
  samples = newSamples;
}

}  // namespace pf::ui::ig
