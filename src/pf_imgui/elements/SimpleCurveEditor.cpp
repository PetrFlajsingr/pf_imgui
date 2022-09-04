//
// Created by xflajs00 on 02.04.2022.
//

#include "SimpleCurveEditor.h"

namespace pf::ui::ig {

SimpleCurveEditor::SimpleCurveEditor(SimpleCurveEditor::Config &&config)
    : ElementWithID(config.name), ValueObservable(getViewToCurveData()), size(config.size), label(config.label.value),
      curveData(config.maxPointCount + 1, ImVec2{0, 0}) {}

SimpleCurveEditor::SimpleCurveEditor(const std::string &elementName, const std::string &labelText, Size s,
                                     std::size_t maxPointCount)
    : ElementWithID(elementName), ValueObservable(getViewToCurveData()), size(s), label(labelText),
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
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  if (ImGui::Curve(label->get().c_str(), static_cast<ImVec2>(*size), static_cast<int>(getMaxPointCount()),
                   curveData.data())) {
    setValueInner(getViewToCurveData());
    notifyValueChanged();
  }
}

CurvePointsView SimpleCurveEditor::getViewToCurveData() const {
  return curveData | ranges::views::take_while(details::IsValidCurvePoint{});
}

}  // namespace pf::ui::ig