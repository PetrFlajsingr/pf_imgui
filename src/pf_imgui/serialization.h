/**
 * @file serialization.h
 * @brief Serialization of UI tree to toml.
 * @author Petr Flajšingr
 * @date 2.11.20
 */
#ifndef PF_IMGUI_SERIALIZATION_H
#define PF_IMGUI_SERIALIZATION_H

#include <glm/gtc/quaternion.hpp>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Layout.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/ui_tree_traversal.h>
#include <ranges>
#include <toml++/toml.h>

namespace pf::ui::ig {

/**
 * Traverse UI tree and serialize all Savable elements.
 * @param root root node of the tree
 * @return toml data containing serialized data
 *
 * @todo: constify this
 */
PF_IMGUI_EXPORT inline toml::table serializeImGuiTree(Renderable &root) {
  auto result = toml::table();
  traverseImGuiTree(root, [&result](Renderable &renderable) {
    if (auto ptrSavable = dynamic_cast<Savable *>(&renderable); ptrSavable != nullptr) {
      if (auto ptrElement = dynamic_cast<Element *>(&renderable); ptrElement != nullptr && ptrSavable->isPersistent()) {
        const auto serialised = ptrSavable->toToml();
        result.insert_or_assign(ptrElement->getName(), serialised);
      }
    }
  });
  return result;
}

/**
 * Deserialize all types of glm::vec
 * @tparam T one of glm::vec types
 * @param arr toml data to deserialize
 * @return deserialized value
 */
template<typename T>
PF_IMGUI_EXPORT T deserializeGlmVec(const toml::array &arr) {
  using ValueType = typename T::value_type;
  auto result = T{};
  for (auto i : std::views::iota(0, T::length())) {
    if constexpr (std::is_floating_point_v<ValueType>) {
      result[i] = static_cast<ValueType>(**arr.get(i)->as_floating_point());
    } else if constexpr (std::integral<ValueType>) {
      result[i] = static_cast<ValueType>(**arr.get(i)->as_integer());
    } else if constexpr (std::same_as<bool, ValueType>) {
      result[i] = **arr.get(i)->as_boolean();
    }
  }
  return result;
}

/**
 * Deserialize all types of glm::vec safely
 * @tparam T one of glm::vec types
 * @param arr toml data to deserialize
 * @return deserialized value
 */
template<typename T>
PF_IMGUI_EXPORT std::optional<T> safeDeserializeGlmVec(const toml::array &arr) {
  using ValueType = typename T::value_type;
  if (static_cast<std::size_t>(T::length()) != arr.size()) { return std::nullopt; }
  auto result = T{};
  for (auto i : std::views::iota(0, T::length())) {
    if constexpr (std::is_floating_point_v<ValueType>) {
      if (const auto value = arr.get(i)->as_floating_point(); value != nullptr) {
        result[i] = static_cast<ValueType>(value->get());
      } else {
        return std::nullopt;
      }
    } else if constexpr (std::integral<ValueType>) {
      if (const auto value = arr.get(i)->as_integer(); value != nullptr) {
        result[i] = static_cast<ValueType>(value->get());
      } else {
        return std::nullopt;
      }
    } else if constexpr (std::same_as<bool, ValueType>) {
      if (const auto value = arr.get(i)->as_boolean(); value != nullptr) {
        result[i] = static_cast<ValueType>(value->get());
      } else {
        return std::nullopt;
      }
    }
  }
  return result;
}
/**
 * Deserialize glm::quat
 * @param arr toml data to deserialize
 * @return deserialized value
 */
PF_IMGUI_EXPORT inline std::optional<glm::quat> safeDeserializeGlmQuat(const toml::array &arr) {
  if (static_cast<std::size_t>(glm::quat::length()) != arr.size()) { return std::nullopt; }
  auto result = glm::quat{};
  for (auto i : std::views::iota(0, glm::quat::length())) {
    if (const auto value = arr.get(i)->as_floating_point(); value != nullptr) {
      result[i] = static_cast<float>(value->get());
    } else {
      return std::nullopt;
    }
  }
  return result;
}
/**
 * Serialize all types of glm::vec
 * @tparam T one of glm::vec types
 * @param vec data to serialize
 * @return serialized data as toml
 */
template<typename T>
PF_IMGUI_EXPORT toml::array serializeGlmVec(const T &vec) {
  auto result = toml::array{};
  for (auto i : std::views::iota(0, T::length())) { result.push_back(vec[i]); }
  return result;
}
/**
 * Serialize glm::quat
 * @param quat data to serialize
 * @return serialized data as toml
 */
PF_IMGUI_EXPORT inline toml::array serializeGlmQuat(const glm::quat &quat) {
  auto result = toml::array{};
  for (auto i : std::views::iota(0, glm::quat::length())) { result.push_back(quat[i]); }
  return result;
}

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SERIALIZATION_H
