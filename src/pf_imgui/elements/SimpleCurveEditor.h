/**
* @file SimpleCurveEditor.h
* @brief A simple smoothing curve editor.
* @author Petr Flajšingr
* @date 2.4.22
*/

#ifndef PF_IMGUI_SIMPLECURVEEDITOR_H
#define PF_IMGUI_SIMPLECURVEEDITOR_H

#include <ImGuiCurveEditor.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/zip.hpp>

namespace pf::ui::ig {
namespace details {
struct IsValidCurvePoint {
  bool operator()(const ImVec2 &point) { return point.x != -1; }
};
}  // namespace details

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

class SimpleCurveEditor : public Element,
                          public Resizable,
                          public Labellable,
                          public ValueObservable<CurvePointsView, CurvePointsViewComparator>,
                          public AllColorCustomizable,
                          public AllStyleCustomizable {
 public:
  SimpleCurveEditor(const std::string &name, Size s, const std::string &label, std::size_t maxPointCount);

  void setMaxPointCount(std::size_t count);
  [[nodiscard]] std::size_t getMaxPointCount() const;

 protected:
  void renderImpl() override;

 private:
  [[nodiscard]] CurvePointsView getViewToCurveData() const;
  std::vector<ImVec2> curveData;
};

}  // namespace pf::ui::ig
#endif  //PF_IMGUI_SIMPLECURVEEDITOR_H
