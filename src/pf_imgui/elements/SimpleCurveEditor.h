/**
 * @file SimpleCurveEditor.h
 * @brief A simple smoothing curve editor.
 * @author Petr Flajšingr
 * @date 2.4.22
 */

#ifndef PF_IMGUI_ELEMENTS_SIMPLECURVEEDITOR_H
#define PF_IMGUI_ELEMENTS_SIMPLECURVEEDITOR_H

#include <ImGuiCurveEditor.h>
#include <glm/vec2.hpp>
#include <pf_common/Explicit.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/reactive/Observable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/zip.hpp>

namespace pf::ui::ig {
namespace details {
struct IsValidCurvePoint {
  bool operator()(const glm::vec2 &point) { return point.x != -1; }  //-V550
};
}  // namespace details

/**
 * Type for range of curve points.
 */
using CurvePointsView =
    ranges::take_while_view<ranges::ref_view<const std::vector<glm::vec2>>, details::IsValidCurvePoint>;

/**
 * @brief Simple curve editor allowing mouse control of key points and basic curve smoothing.
 */
class SimpleCurveEditor : public ElementWithID, public ValueContainer<CurvePointsView, ReadOnlyTag>, public Savable {
 public:
  /**
   * @brief Construction config for SimpleCurveEditor
   */
  struct Config {
    using Parent = SimpleCurveEditor;
    Explicit<std::string_view> name;     /*!< Unique name of the element */
    Explicit<std::string_view> label;    /*!< Label rendered as overlay over the editor */
    Size size = Size::Auto();            /*!< Size of the element */
    Explicit<std::size_t> maxPointCount; /*!< Maximum allowed key points */
    bool persistent = false;             /*!< Enable/disable state saving */
  };

  /**
   * Construct SimpleCurveEditor
   * @param config construction args @see SimpleCurveEditor::Config
   */
  explicit SimpleCurveEditor(Config &&config);
  /**
   * Construct SimpleCurveEditor
   * @param elementName unique name of the element
   * @param labelText label rendered as overlay over the editor
   * @param s size of the element
   * @param maxPointCount maximum allowed key points
   * @param persistent enable/disable state saving
   */
  SimpleCurveEditor(std::string_view elementName, std::string_view labelText, Size s, std::size_t maxPointCount,
                    Persistent persistent = Persistent::No);

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

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  [[nodiscard]] const CurvePointsView &getValue() const override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const CurvePointsView &)> listener) override;

 public:
  FullColorPalette color;
  FullStyleOptions style;
  Observable<Label> label;

  Observable<Size> size;

 private:
  std::vector<glm::vec2> curveData;

 public:
  ObservableProperty<SimpleCurveEditor, CurvePointsView, ReadOnlyTag, AlwaysTrueChangeDetector> curvePoints;

 protected:
  void renderImpl() override;

 private:
  [[nodiscard]] CurvePointsView getViewToCurveData() const;
};

}  // namespace pf::ui::ig
#endif  //PF_IMGUI_ELEMENTS_SIMPLECURVEEDITOR_H
