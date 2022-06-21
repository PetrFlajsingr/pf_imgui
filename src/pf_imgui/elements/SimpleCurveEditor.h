/**
 * @file SimpleCurveEditor.h
 * @brief A simple smoothing curve editor.
 * @author Petr Flajšingr
 * @date 2.4.22
 */

#ifndef PF_IMGUI_ELEMENTS_SIMPLECURVEEDITOR_H
#define PF_IMGUI_ELEMENTS_SIMPLECURVEEDITOR_H

#include <ImGuiCurveEditor.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/zip.hpp>

namespace pf::ui::ig {
namespace details {
struct IsValidCurvePoint {
  bool operator()(const ImVec2 &point) { return point.x != -1; }  //-V550
};
}  // namespace details

/**
 * Type for range of curve points.
 */
using CurvePointsView =
    ranges::take_while_view<ranges::ref_view<const std::vector<ImVec2>>, details::IsValidCurvePoint>;
struct CurvePointsViewComparator {
  [[nodiscard]] bool operator()(CurvePointsView lhs, CurvePointsView rhs) {
    return std::ranges::all_of(ranges::views::zip(lhs, rhs), [](const auto &val) {
      const auto &[lhs, rhs] = val;
      return lhs.x == rhs.x && lhs.y == rhs.y;
    });
  }
};

/**
 * @brief Simple curve editor allowing mouse control of key points and basic curve smoothing.
 */
class SimpleCurveEditor : public ElementWithID,
                          public Resizable,
                          public Labellable,
                          public ValueObservable<CurvePointsView, CurvePointsViewComparator> {
 public:
  /**
   * @brief Construction config for SimpleCurveEditor
   */
  struct Config {
    using Parent = SimpleCurveEditor;
    Explicit<std::string> name;          /*!< Unique name of the element */
    Explicit<std::string> label;         /*!< Label rendered as overlay over the editor */
    Size size = Size::Auto();            /*!< Size of the element */
    Explicit<std::size_t> maxPointCount; /*!< Maximum allowed key points */
  };

  /**
   * Construct SimpleCurveEditor
   * @param config construction args @see SimpleCurveEditor::Config
   */
  explicit SimpleCurveEditor(Config &&config);
  /**
   * Construct SimpleCurveEditor
   * @param name unique name of the element
   * @param label label rendered as overlay over the editor
   * @param s size of the element
   * @param maxPointCount maximum allowed key points
   */
  SimpleCurveEditor(const std::string &name, const std::string &label, Size s, std::size_t maxPointCount);

  /**
   * Set maximum allowed key points.
   * @param count new value
   */
  void setMaxPointCount(std::size_t count);
  /**
   * Get maximum allowed key points.
   */
  [[nodiscard]] std::size_t getMaxPointCount() const;

  /**
   * Calculate curve value on given position.
   * @param x X axis position in range of [0.0, 1.0]
   * @return curve value at given position
   */
  [[nodiscard]] float getCurveValue(float x) const;
  /**
   * Calculate smoothed curve value on given position.
   * @param x X axis position in range of [0.0, 1.0]
   * @return smoothed curve value at given position
   */
  [[nodiscard]] float getSmoothCurveValue(float x) const;

  FullColorPalette color;
  FullStyleOptions style;

 protected:
  void renderImpl() override;

 private:
  [[nodiscard]] CurvePointsView getViewToCurveData() const;
  std::vector<ImVec2> curveData;
};

}  // namespace pf::ui::ig
#endif  //PF_IMGUI_ELEMENTS_SIMPLECURVEEDITOR_H
