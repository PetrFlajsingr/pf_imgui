/**
 * @file Gizmo3D.h
 * @brief Gizmo3D element.
 * @author Petr Flajšingr
 * @date 17.11.21
 */

#ifndef PF_IMGUI_GIZMO3D_H
#define PF_IMGUI_GIZMO3D_H

#include <algorithm>
#include <fmt/format.h>
#include <imGuIZMOquat.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>
#include <utility>

namespace pf::ui::ig {
enum class GizmoType {
  Axes3 = imguiGizmo::mode3Axes,
  Direction = imguiGizmo::modeDirection,
  Dual = imguiGizmo::mode3Axes | imguiGizmo::modeDirection
};
enum class GizmoMid { Cube = imguiGizmo::cubeAtOrigin, Sphere = imguiGizmo::sphereAtOrigin, None = 0 };

namespace details {
template<GizmoType Type>
using GizmoValueType =
    std::conditional_t<Type == GizmoType::Axes3, glm::quat,
                       std::conditional_t<Type == GizmoType::Direction, glm::vec3, std::pair<glm::quat, glm::vec3>>>;
}

/**
 * @todo Savable
 * @brief A 3D gizmo allowing the user to modify rotation or a direction by rendering axis'/direction arrow.
 * Axes3 -> 3D rotation
 * Direction -> 3D direction
 * Dual -> both of the above
 * @tparam Type
 */
template<GizmoType Type>
class PF_IMGUI_EXPORT Gizmo3D : public Element,
                                public Resizable,
                                public ValueObservable<details::GizmoValueType<Type>> {
  using ValueType = details::GizmoValueType<Type>;

 public:
  /**
   * @brief Struct for construction of Gizmo3D.
   */
  struct Config {
    using Parent = Gizmo3D;
    std::string_view name;                                  /*!< Unique name of the element */
    ValueType value;                                        /*!< Initial value */
    Size size = Size{IMGUIZMO_DEF_SIZE, IMGUIZMO_DEF_SIZE}; /*!< Size of the element */
  };
  /**
   * Construct Gizmo3D
   * @param config construction args @see Gizmo3D::Config
   */
  explicit Gizmo3D(Config &&config);
  /**
   * Construct Gizmo3D.
   * @param name unique name of the element
   * @param value starting value - needs to be valid, otherwise the element will malfunction
   * @param size size of the element, the smaller value will be used for both axis'
   */
  Gizmo3D(const std::string &name, ValueType value, const Size &size = Size{IMGUIZMO_DEF_SIZE, IMGUIZMO_DEF_SIZE});

  /**
   * Set a type of object rendered in the origin.
   * @param newObject type of the object to render
   */
  void setMidObject(GizmoMid newObject)
    requires(Type != GizmoType::Direction)
  ;

  void setSize(Size s) override;

 protected:
  void renderImpl() override;

 private:
  GizmoMid mid = GizmoMid::None;
};

template<GizmoType Type>
Gizmo3D<Type>::Gizmo3D(Gizmo3D::Config &&config)
    : Element(std::string{config.name}), Resizable(config.size), ValueObservable<ValueType>(config.value) {}

template<GizmoType Type>
Gizmo3D<Type>::Gizmo3D(const std::string &name, Gizmo3D::ValueType value, const Size &size)
    : Element(name), Resizable(size), ValueObservable<ValueType>(value) {}

template<GizmoType Type>
void Gizmo3D<Type>::setMidObject(GizmoMid newObject)
  requires(Type != GizmoType::Direction)
{
  mid = newObject;
}

template<GizmoType Type>
void Gizmo3D<Type>::setSize(Size s) {
  const auto min = std::min(static_cast<float>(s.width), static_cast<float>(s.height)());
  Resizable::setSize(Size{min, min});
}

template<GizmoType Type>
void Gizmo3D<Type>::renderImpl() {
  const auto flags = static_cast<int>(Type) | static_cast<int>(mid);
  if constexpr (Type == GizmoType::Axes3) {
    glm::quat quat = ValueObservable<ValueType>::getValue();
    if (ImGui::gizmo3D(fmt::format("##{}", getName()).c_str(), quat, getSize().width.value, flags)) {
      ValueObservable<ValueType>::setValueAndNotifyIfChanged(quat);
    }
  }
  if constexpr (Type == GizmoType::Direction) {
    glm::vec3 dir = ValueObservable<ValueType>::getValue();
    if (ImGui::gizmo3D(fmt::format("##{}", getName()).c_str(), dir, getSize().width.value, flags)) {
      ValueObservable<ValueType>::setValueAndNotifyIfChanged(dir);
    }
  }
  if constexpr (Type == GizmoType::Dual) {
    auto [quat, dir] = ValueObservable<ValueType>::getValue();
    if (ImGui::gizmo3D(fmt::format("##{}", getName()).c_str(), quat, dir, getSize().width.value, flags)) {
      ValueObservable<ValueType>::setValueAndNotifyIfChanged({quat, dir});
    }
  }
}

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_GIZMO3D_H
