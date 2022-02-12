/**
 * @file SimplePlot.h
 * @brief SimplePlot element.
 * @author Petr Flajšingr
 * @date 1.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOT_H
#define PF_IMGUI_ELEMENTS_PLOT_H

#include <algorithm>
#include <optional>
#include <pf_imgui/_export.h>
#include <ranges>
#include <src/pf_imgui/interface/Customizable.h>
#include <src/pf_imgui/interface/Element.h>
#include <src/pf_imgui/interface/Labellable.h>
#include <src/pf_imgui/interface/Resizable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief A simple plot which can visualize float values in 2 ways.
 *
 * @see PlotType
 */
class PF_IMGUI_EXPORT SimplePlot
    : public Element,
      public Labellable,
      public Resizable,
      public ColorCustomizable<style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::PlotLines,
                               style::ColorOf::PlotLinesHovered, style::ColorOf::PlotHistogram,
                               style::ColorOf::PlotHistogramHovered, style::ColorOf::Text,
                               style::ColorOf::TextDisabled> {
 public:
  /**
   * @brief Struct for construction of SimplePlot.
   */
  struct Config {
    using Parent = SimplePlot;
    std::string_view name;                             /*!< Unique name of the element */
    std::string_view label;                            /*!< Text rendered above the plot */
    PlotType type;                                     /*!< Type of the plot @see PlotType */
    std::vector<float> values{};                       /*!< Initial values displayed on the plot */
    std::optional<std::string> overlay = std::nullopt; /*!< Text rendered on top of the plot */
    std::optional<std::size_t> maxHistoryCount =
        std::nullopt;          /*!< Maximum amount of values rendered in the plot FIFO */
    float scaleLow = FLT_MAX;  /*!< Lowest displayed value */
    float scaleHigh = FLT_MAX; /*!< Highest displayed value */
    Size size = Size::Auto();  /*!< Size of the element */
  };
  /**
   * Construct SimplePlot.
   * @param config construction args @see SimplePlot::Config
   */
  explicit SimplePlot(Config &&config);
  /**
   * Construct SimplePlot.
   * @param elementName ID of the plot
   * @param text rendered above the plot
   * @param plotType type of the plot @see PlotType
   * @param values starting values of the plot
   * @param overlayText text rendered on top of the plot
   * @param historyLimit max count of values in plot, older ones are deleted first (FIFO)
   * @param scaleLow min value
   * @param scaleHigh max value
   * @param size size of the element
   */
  SimplePlot(const std::string &elementName, const std::string &label, PlotType plotType,
             std::vector<float> values = {}, std::optional<std::string> overlayText = std::nullopt,
             const std::optional<std::size_t> &historyLimit = std::nullopt, float scaleLow = FLT_MAX,
             float scaleHigh = FLT_MAX, Size size = Size::Auto());
  /**
   * Add value to the end of the graph.
   * @param value value to be added
   */
  void addValue(float value);
  /**
   * Add values to the end of the graph.
   * @param values values to be added
   */
  void addValue(const std::ranges::range auto &vals) requires(
      std::same_as<float, std::ranges::range_value_t<decltype(vals)>>) {
    std::ranges::copy(vals, std::back_inserter(values));
  }
  /**
   * Remove all values from the graph.
   */
  void clear();
  /**
   * Set new values, overwriting the old ones.
   */
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
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_PLOT_H
