//
// Created by xflajs00 on 22.05.2022.
//

#include "OverlayGizmoBase.h"
#include <glm/gtc/type_ptr.hpp>

namespace pf::ui::ig {

OverlayGizmoBase::OverlayGizmoBase(const glm::mat4 &initialValue, ViewportGizmoMode gizmoMode,
                                   ViewportGizmoSpace gizmoSpace, const glm::mat4 &initialView,
                                   const glm::mat4 &initialProjection, bool isProjectionOrthographic)
    : transform(initialValue), mode(gizmoMode), space(gizmoSpace), view(initialView), projection(initialProjection),
      projectionOrthographic(isProjectionOrthographic) {}

ViewportGizmoMode OverlayGizmoBase::getMode() const { return mode; }

void OverlayGizmoBase::setMode(ViewportGizmoMode newMode) { mode = newMode; }

ViewportGizmoSpace OverlayGizmoBase::getSpace() const { return space; }

void OverlayGizmoBase::setSpace(ViewportGizmoSpace newSpace) { space = newSpace; }

const glm::mat4 &OverlayGizmoBase::getView() const { return view; }

void OverlayGizmoBase::setView(const glm::mat4 &newView) { view = newView; }

const glm::mat4 &OverlayGizmoBase::getProjection() const { return projection; }

void OverlayGizmoBase::setProjection(const glm::mat4 &newProjection, bool isOrthographic) {
  projection = newProjection;
  projectionOrthographic = isOrthographic;
}

void OverlayGizmoBase::setSnapValues(float translate, float rotate, float scale) {
  snapValues = glm::vec3{translate, rotate, scale};
}

void OverlayGizmoBase::drawImpl(bool isEnabled) {
  ImGuizmo::Enable(isEnabled);
  // TODO: use this as another overlay element
  /*const auto identity = glm::mat4{1.f};

  ImGuizmo::DrawGrid(glm::value_ptr(view), glm::value_ptr(projection), glm::value_ptr(identity), 100.f);
  ImGuizmo::DrawCubes(glm::value_ptr(view), glm::value_ptr(projection), glm::value_ptr(*getValueAddress()), 1);*/
  if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), static_cast<ImGuizmo::OPERATION>(mode),
                           static_cast<ImGuizmo::MODE>(space), glm::value_ptr(transform.value), nullptr,
                           snapValues.has_value() ? glm::value_ptr(*snapValues) : nullptr)) {
    transform.triggerListeners();
  }
  const auto isUsing = ImGuizmo::IsUsing();
  if (isUsing && !wasUsing) {
    *inUse.modify() = true;
    wasUsing = true;
  } else if (!isUsing && wasUsing) {
    *inUse.modify() = false;
    wasUsing = false;
  }
}

void OverlayGizmoBase::setValue(const glm::mat4 &newValue) { *transform.modify() = newValue; }

const glm::mat4 &OverlayGizmoBase::getValue() const { return *transform; }

Subscription OverlayGizmoBase::addValueListenerImpl(std::function<void(const glm::mat4 &)> listener) {
  return transform.addListener(std::move(listener));
}

}  // namespace pf::ui::ig