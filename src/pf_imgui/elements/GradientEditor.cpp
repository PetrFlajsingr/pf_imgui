//
// Created by xflajs00 on 03.04.2022.
//

#include "GradientEditor.h"

namespace pf::ui::ig {

bool GradientPoint::operator==(const GradientPoint &rhs) const {
  return color == rhs.color && position == rhs.position;
}

std::optional<GradientPoint> GradientPoint::FromToml(const toml::table &src) {
  GradientPoint result;
  if (auto colorIter = src.find("color"); colorIter != src.end()) {
    if (auto colorArr = colorIter->second.as_array(); colorArr != nullptr && colorArr->size() == 4) {
      result.color = Color::RGB((*colorArr)[0].value_or(1.f), (*colorArr)[1].value_or(1.f),
                                (*colorArr)[2].value_or(1.f), (*colorArr)[3].value_or(1.f));
    } else {
      return std::nullopt;
    }
  } else {
    return std::nullopt;
  }
  if (auto posIter = src.find("position"); posIter != src.end()) {
    if (auto posVal = posIter->second.value<float>(); posVal.has_value()) {
      result.position = posVal.value();
    } else {
      return std::nullopt;
    }
  } else {
    return std::nullopt;
  }
  return result;
}

toml::table GradientPoint::toToml() const {
  return toml::table{{"color", toml::array{color.red(), color.green(), color.blue(), color.alpha()}},
                     {"position", position}};
}

GradientPoint details::GradientMarkToGradientPoint::operator()(ImGradientMark *mark) const {
  return {Color::RGB(mark->color[0], mark->color[1], mark->color[2], mark->color[3]), mark->position};
}

bool GradientPointsViewComparator::operator()(GradientPointsView lhs, GradientPointsView rhs) {
  return std::ranges::all_of(ranges::views::zip(lhs, rhs), [](const auto &val) {
    const auto &[lhs, rhs] = val;
    return lhs == rhs;
  });
}

GradientEditor::GradientEditor(GradientEditor::Config &&config)
    : Element(std::string{config.name}), ValueObservable(getPointsView()),
      Savable(config.persistent ? Persistent::Yes : Persistent::No) {}

GradientEditor::GradientEditor(const std::string &name, Persistent persistent)
    : Element(name), ValueObservable(getPointsView()), Savable(persistent) {}

Color GradientEditor::getColorAt(float percentage) const {
  float color[4];
  gradient.getColorAt(percentage, color);
  return Color::RGB(color[0], color[1], color[2], color[3]);
}

void GradientEditor::addGradientPoint(GradientPoint gradientPoint) {
  gradient.addMark(gradientPoint.position, static_cast<ImVec4>(gradientPoint.color));
}

void GradientEditor::removeGradientPoint(GradientPoint gradientPoint) {
  auto points = getPointsView();
  auto iter = points.begin();
  for (; iter != points.end(); ++iter) {
    if (*iter == gradientPoint) { break; }
  }
  if (iter == points.end()) { return; }
  const auto removeIndex = std::ranges::distance(points.begin(), iter);
  const auto removeIter = std::ranges::next(gradient.getMarks().begin(), removeIndex);
  gradient.removeMark(*removeIter);
}

void GradientEditor::renderImpl() {
  if (ImGui::GradientEditor(&gradient, draggingMark, selectedMark)) {
    setValueInner(getPointsView());
    notifyValueChanged();
  }
  setHovered(gradient.hovered);
  updateFocused(gradient.focused);
  if (hasFocus() && ImGui::IsKeyPressed(ImGuiKey_Delete) && selectedMark != nullptr) {
    gradient.removeMark(selectedMark);
    setValueInner(getPointsView());
    notifyValueChanged();
  }
}

void GradientEditor::unserialize_impl(const toml::table &src) {
  if (auto pointsIter = src.find("points"); pointsIter != src.end()) {
    if (auto pointsArr = pointsIter->second.as_array(); pointsArr != nullptr) {
      std::ranges::for_each(*pointsArr, [this](const auto &pointToml) {
        if (auto pointTable = pointToml.as_table(); pointTable != nullptr) {
          if (auto point = GradientPoint::FromToml(*pointTable); point.has_value()) { addGradientPoint(point.value()); }
        }
      });
    }
  }
}

toml::table GradientEditor::serialize_impl() const {
  auto points = toml::array{};
  std::ranges::for_each(getPointsView(), [&points](const auto &point) { points.push_back(point.toToml()); });
  return toml::table{{"points", points}};
}

GradientPointsView GradientEditor::getPointsView() const {
  return gradient.getMarks() | ranges::views::transform(details::GradientMarkToGradientPoint{});
}

}  // namespace pf::ui::ig
