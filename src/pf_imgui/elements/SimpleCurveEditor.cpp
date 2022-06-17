//
// Created by xflajs00 on 02.04.2022.
//

#include "SimpleCurveEditor.h"

namespace pf::ui::ig {

SimpleCurveEditor::SimpleCurveEditor(SimpleCurveEditor::Config &&config)
    : ElementWithID(config.name), Resizable(config.size), Labellable(config.label), ValueObservable(getViewToCurveData()),
      curveData(config.maxPointCount + 1, ImVec2{0, 0}) {}

SimpleCurveEditor::SimpleCurveEditor(const std::string &name, const std::string &label, Size s,
                                     std::size_t maxPointCount)
    : ElementWithID(name), Resizable(s), Labellable(label), ValueObservable(getViewToCurveData()),
      curveData(maxPointCount + 1, ImVec2{0, 0}) {}

void SimpleCurveEditor::setMaxPointCount(std::size_t count) { curveData.resize(count + 1); }

std::size_t SimpleCurveEditor::getMaxPointCount() const { return curveData.size() - 1; }

float SimpleCurveEditor::getCurveValue(float x) const {
  return ImGui::CurveValue(x, static_cast<int>(curveData.size() - 1), curveData.data());
}

float SimpleCurveEditor::getSmoothCurveValue(float x) const {
  return ImGui::CurveValueSmooth(x, static_cast<int>(curveData.size() - 1), curveData.data());
}

void SimpleCurveEditor::renderImpl() {
  [[maybe_unused]] auto styleRAII = setStyleStack();
  [[maybe_unused]] auto colorRAII = setColorStack();
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