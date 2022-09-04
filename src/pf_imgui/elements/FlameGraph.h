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
#include <pf_common/Explicit.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/reactive/Observable.h>
#include <pf_imgui/style/ColorPalette.h>
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
 * @todo: styles for samples?
 */
class PF_IMGUI_EXPORT FlameGraph : public ElementWithID {
 public:
  /**
   * @brief Struct for construction of FlameGraph.
   */
  struct Config {
    using Parent = FlameGraph;
    Explicit<std::string_view> name;                   /*!< Unique name of the element */
    Explicit<std::string_view> label;                  /*!< Text rendered next to the graph */
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
   * @param labelText text drawn at the top of the graph
   * @param initialSize size of the graph
   * @param graphOverlay text render on top of the graph
   */
  FlameGraph(const std::string &elementName, const std::string &labelText, const Size &initialSize = Size::Auto(),
             std::optional<std::string> graphOverlay = std::nullopt);

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

  ColorPalette<ColorOf::PlotHistogram, ColorOf::PlotHistogramHovered, ColorOf::FrameBackground, ColorOf::Border,
               ColorOf::BorderShadow, ColorOf::Text>
      color;
  Font font = Font::Default();
  Observable<Label> label;

  Observable<Size> size;

 protected:
  void renderImpl() override;

 private:
  std::optional<std::string> overlay;
  std::vector<FlameGraphSample> samples;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_FLAMEGRAPH_H
