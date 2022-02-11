/**
 * @file FlameGraph.h
 * @brief FlameGraph element.
 * @author Petr Flajšingr
 * @date 7.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_FLAMEGRAPH_H
#define PF_IMGUI_ELEMENTS_FLAMEGRAPH_H

#include <imgui_widget_flamegraph.h>
#include <optional>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Sample for flame graph.
 */
using FlameGraphSample = ImGuiWidgetFlameGraph::FlameGraphSample;
/**
 * @brief Graph for visualising stack traces, performance stuff...
 * @todo: style
 */
class PF_IMGUI_EXPORT FlameGraph
    : public Element,
      public Labellable,
      public Resizable,
      public ColorCustomizable<style::ColorOf::PlotHistogram, style::ColorOf::PlotHistogramHovered,
                               style::ColorOf::FrameBackground, style::ColorOf::Border, style::ColorOf::BorderShadow,
                               style::ColorOf::Text> {
 public:
  /**
   * @brief Struct for construction of FlameGraph.
   */
  struct Config {
    using Parent = FlameGraph;
    std::string_view name;                             /*!< Unique name of the element */
    std::string_view label;                            /*!< Text rendered next to the graph */
    Size size = Size::Auto();                          /*!< Size of the element */
    std::optional<std::string> overlay = std::nullopt; /*!< Text rendered on top of the graph */
  };
  /**
   * Construct FlameGraph
   * @param config construction args @see FlameGraph::Config
   */
  explicit FlameGraph(Config &&config);
  /**
   * Construct FlameGraph.
   * @param elementName ID of the element
   * @param label text drawn at the top of the graph
   * @param size size of the graph
   * @param overlay text render on top of the graph
   */
  FlameGraph(const std::string &elementName, const std::string &label, const Size &size = Size::Auto(),
             std::optional<std::string> overlay = std::nullopt);

  /**
   * Set text render on top of the graph.
   * @param text text to be rendered
   */
  void setOverlay(std::string text);
  /**
   * Disable overlay rendering.
   */
  void disableOverlay();

  /**
   * Set samples from which the flame graph will be drawn.
   * @param newSamples samples
   */
  void setSamples(const std::vector<FlameGraphSample> &newSamples);

 protected:
  void renderImpl() override;

 private:
  std::optional<std::string> overlay;
  std::vector<FlameGraphSample> samples;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_FLAMEGRAPH_H
