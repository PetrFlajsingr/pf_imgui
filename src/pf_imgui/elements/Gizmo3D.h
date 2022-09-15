/**
 * @file Gizmo3D.h
 * @brief Gizmo3D element.
 * @author Petr Flajšingr
 * @date 17.11.21
 */

#ifndef PF_IMGUI_ELEMENTS_GIZMO3D_H
#define PF_IMGUI_ELEMENTS_GIZMO3D_H

#include <algorithm>
#include <fmt/format.h>
#include <imGuIZMOquat.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
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
 * @brief A 3D gizmo allowing the user to modify rotation or a direction by rendering axis'/direction arrow.
 * Axes3 -> 3D rotation
 * Direction -> 3D direction
 * Dual -> both of the above
 * @tparam Type
 */
template<GizmoType Type>
class PF_IMGUI_EXPORT Gizmo3D : public ItemElement,
                                public ValueContainer<details::GizmoValueType<Type>>,
                                public Savable {
  using ValueType = details::GizmoValueType<Type>;

 public:
  /**
   * @brief Struct for construction of Gizmo3D.
   */
  struct Config {
    using Parent = Gizmo3D;
    Explicit<std::string_view> name;                        /*!< Unique name of the element */
    Explicit<ValueType> value;                              /*!< Initial value */
    Size size = Size{IMGUIZMO_DEF_SIZE, IMGUIZMO_DEF_SIZE}; /*!< Size of the element */
    bool persistent = false;                                /*!< Enable state saving to disk */
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
   * @param persistent enable state saving
   */
  Gizmo3D(std::string_view name, ValueType value, Size s = Size{IMGUIZMO_DEF_SIZE, IMGUIZMO_DEF_SIZE},
          Persistent persistent = Persistent::No);

  /**
   * Set a type of object rendered in the origin.
   * @param newObject type of the object to render
   */
  void setMidObject(GizmoMid newObject)
    requires(Type != GizmoType::Direction);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  Observable<Size> size;
  Observable<ValueType> value;

  void setValue(const details::GizmoValueType<Type> &newValue) override;
  [[nodiscard]] const details::GizmoValueType<Type> &getValue() const override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const details::GizmoValueType<Type> &)> listener) override;

  void renderImpl() override;

 private:
  GizmoMid mid = GizmoMid::None;
};

template<GizmoType Type>
Gizmo3D<Type>::Gizmo3D(Gizmo3D::Config &&config)
    : ItemElement(config.name.value), Savable(config.persistent ? Persistent::Yes : Persistent::No), size(config.size),
      value(config.value) {
  size.addListener([this](Size newSize) {
    const auto min = std::min(static_cast<float>(newSize.width), static_cast<float>(newSize.height));
    const auto fixedSize = Size{min, min};
    if (fixedSize != newSize) { *size.modify() = fixedSize; }
  });
}

template<GizmoType Type>
Gizmo3D<Type>::Gizmo3D(std::string_view name, Gizmo3D::ValueType value, Size s, Persistent persistent)
    : ItemElement(name), Savable(persistent), size(s), value(value) {
  size.addListener([this](Size newSize) {
    const auto min = std::min(static_cast<float>(newSize.width), static_cast<float>(newSize.height));
    const auto fixedSize = Size{min, min};
    if (fixedSize != newSize) { *size.modify() = fixedSize; }
  });
}

template<GizmoType Type>
void Gizmo3D<Type>::setMidObject(GizmoMid newObject)
  requires(Type != GizmoType::Direction)
{
  mid = newObject;
}

template<GizmoType Type>
toml::table Gizmo3D<Type>::toToml() const {
  auto result = toml::table{};
  if constexpr (Type == GizmoType::Direction) { result.insert("direction", serializeGlmVec(*value)); }
  if constexpr (Type == GizmoType::Axes3) { result.insert("quaternion", serializeGlmQuat(*value)); }
  if constexpr (Type == GizmoType::Dual) {
    result.insert("quaternion", serializeGlmQuat(value->first));
    result.insert("direction", serializeGlmVec(value->second));
  }
  return result;
}

template<GizmoType Type>
void Gizmo3D<Type>::setFromToml(const toml::table &src) {
  glm::vec3 direction{};
  glm::quat quaternion{};
  if constexpr (Type == GizmoType::Direction || Type == GizmoType::Dual) {
    using VecType = glm::vec3;
    if (auto newValIter = src.find("direction"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        const auto vecValue = safeDeserializeGlmVec<VecType>(*newVal);
        if (vecValue.has_value()) {
          direction = *vecValue;
        } else {
          return;
        }
      }
    }
  }
  if constexpr (Type == GizmoType::Axes3 || Type == GizmoType::Dual) {
    if (auto newValIter = src.find("quaternion"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        const auto quatValue = safeDeserializeGlmQuat(*newVal);
        if (quatValue.has_value()) {
          quaternion = *quatValue;
        } else {
          return;
        }
      }
    }
  }

  if constexpr (Type == GizmoType::Direction) { *value.modify() = direction; }
  if constexpr (Type == GizmoType::Axes3) { *value.modify() = quaternion; }
  if constexpr (Type == GizmoType::Dual) { *value.modify() = std::make_pair(quaternion, direction); }
}

template<GizmoType Type>
void Gizmo3D<Type>::renderImpl() {
  const auto flags = static_cast<int>(Type) | static_cast<int>(mid);
  if constexpr (Type == GizmoType::Axes3) {
    glm::quat quat = *value;
    if (ImGui::gizmo3D(fmt::format("##{}", getName()).c_str(), quat, static_cast<float>(size->width), flags)) {
      *value.modify() = quat;
    }
  }
  if constexpr (Type == GizmoType::Direction) {
    glm::vec3 dir = *value;
    if (ImGui::gizmo3D(fmt::format("##{}", getName()).c_str(), dir, static_cast<float>(size->width), flags)) {
      *value.modify() = dir;
    }
  }
  if constexpr (Type == GizmoType::Dual) {
    auto [quat, dir] = *value;
    if (ImGui::gizmo3D(fmt::format("##{}", getName()).c_str(), quat, dir, static_cast<float>(size->width), flags)) {
      *value.modify() = {quat, dir};
    }
  }
}
template<GizmoType Type>
void Gizmo3D<Type>::setValue(const details::GizmoValueType<Type> &newValue) {
  *value.modify() = newValue;
}

template<GizmoType Type>
const details::GizmoValueType<Type> &Gizmo3D<Type>::getValue() const {
  return *value;
}

template<GizmoType Type>
Subscription Gizmo3D<Type>::addValueListenerImpl(std::function<void(const details::GizmoValueType<Type> &)> listener) {
  return value.addListener(std::move(listener));
}

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_GIZMO3D_H
