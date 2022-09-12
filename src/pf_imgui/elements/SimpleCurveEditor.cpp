//
// Created by xflajs00 on 02.04.2022.
//

#include "SimpleCurveEditor.h"
#include "toml++/toml.h"
#include <pf_imgui/serialization.h>

namespace pf::ui::ig {

SimpleCurveEditor::SimpleCurveEditor(SimpleCurveEditor::Config &&config)
    : ElementWithID(config.name), Savable(config.persistent ? Persistent::Yes : Persistent::No),
      label(config.label.value), size(config.size), curvePoints(getViewToCurveData()),
      curveData(config.maxPointCount + 1, glm::vec2{0, 0}) {}

SimpleCurveEditor::SimpleCurveEditor(const std::string &elementName, const std::string &labelText, Size s,
                                     std::size_t maxPointCount, Persistent persistent)
    : ElementWithID(elementName), Savable(persistent), label(labelText), size(s), curvePoints(getViewToCurveData()),
      curveData(maxPointCount + 1, glm::vec2{0, 0}) {}

void SimpleCurveEditor::setMaxPointCount(std::size_t count) { curveData.resize(count + 1); }

std::size_t SimpleCurveEditor::getMaxPointCount() const { return curveData.size() - 1; }

float SimpleCurveEditor::getCurveValue(float x) const {
  return ImGui::CurveValue(x, static_cast<int>(curveData.size() - 1),
                           reinterpret_cast<const ImVec2 *>(curveData.data()));
}

float SimpleCurveEditor::getSmoothCurveValue(float x) const {
  return ImGui::CurveValueSmooth(x, static_cast<int>(curveData.size() - 1),
                                 reinterpret_cast<const ImVec2 *>(curveData.data()));
}

void SimpleCurveEditor::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  if (ImGui::Curve(label->get().c_str(), static_cast<ImVec2>(*size), static_cast<int>(getMaxPointCount()),
                   reinterpret_cast<ImVec2 *>(curveData.data()))) {
    *curvePoints.modify() = getViewToCurveData();
  }
}

CurvePointsView SimpleCurveEditor::getViewToCurveData() const {
  return curveData | ranges::views::take_while(details::IsValidCurvePoint{});
}

toml::table SimpleCurveEditor::toToml() const {
  auto valuesToml = toml::array{};
  std::ranges::for_each(getViewToCurveData(),
                        [&](glm::vec2 point) { valuesToml.emplace_back<toml::array>(serializeGlmVec(point)); });
  return toml::table{{"values", valuesToml}};
}

void SimpleCurveEditor::setFromToml(const toml::table &src) {
  curveData.clear();
  if (auto valuesIter = src.find("values"); valuesIter != src.end()) {
    if (auto valuesVal = valuesIter->second.as_array(); valuesVal != nullptr) {
      std::ranges::for_each(*valuesVal, [this](auto &point) {
        if (auto pointArr = point.as_array(); pointArr != nullptr) {
          if (pointArr->size() == 2) {
            if (pointArr->get(0)->as_floating_point() && pointArr->get(1)->as_floating_point()) {
              curveData.emplace_back(
                  glm::vec2{**pointArr->get(0)->as_floating_point(), **pointArr->get(1)->as_floating_point()});
            }
          }
        }
      });
    }
  }
}

const CurvePointsView &SimpleCurveEditor::getValue() const { return *curvePoints; }

Subscription SimpleCurveEditor::addValueListenerImpl(std::function<void(const CurvePointsView &)> listener) {
  return curvePoints.addListener(std::move(listener));
}

}  // namespace pf::ui::ig