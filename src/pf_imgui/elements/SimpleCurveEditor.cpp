//
// Created by xflajs00 on 02.04.2022.
//

#include "SimpleCurveEditor.h"

namespace pf::ui::ig {

SimpleCurveEditor::SimpleCurveEditor(const std::string &name, Size s, const std::string &label,
                                     std::size_t maxPointCount)
    : Element(name), Resizable(s), Labellable(label), ValueObservable(getViewToCurveData()),
      curveData(maxPointCount + 1, ImVec2{0, 0}) {}

void SimpleCurveEditor::setMaxPointCount(std::size_t count) { curveData.resize(count + 1); }

std::size_t SimpleCurveEditor::getMaxPointCount() const { return curveData.size() - 1; }

void SimpleCurveEditor::renderImpl() {
  auto styleRAII = setStyleStack();
  auto colorRAII = setColorStack();
  if (ImGui::Curve(getLabel().c_str(), static_cast<ImVec2>(getSize()), static_cast<int>(getMaxPointCount()),
                   curveData.data())) {
    setValueInner(getViewToCurveData());
    notifyValueChanged();
  }
}

CurvePointsView SimpleCurveEditor::getViewToCurveData() const {
  return curveData | ranges::views::take_while(details::IsValidCurvePoint{});
}

}  // namespace pf::ui::ig