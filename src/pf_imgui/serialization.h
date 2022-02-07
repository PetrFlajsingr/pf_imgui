/**
 * @file serialization.h
 * @brief Serialization of UI tree to toml.
 * @author Petr Flajšingr
 * @date 2.11.20
 */
#ifndef PF_IMGUI_SERIALIZATION_H
#define PF_IMGUI_SERIALIZATION_H

#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Layout.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/ui_tree_traversal.h>
#include <ranges>
#include <toml++/toml_array.h>
#include <toml++/toml_table.h>

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
      if (auto ptrElement = dynamic_cast<Element *>(&renderable); ptrElement != nullptr) {
        const auto optSerialised = ptrSavable->serialize();
        if (optSerialised.has_value()) { result.insert_or_assign(ptrElement->getName(), *optSerialised); }
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
  auto result = T{};
  for (auto i : std::views::iota(0, T::length())) {
    if constexpr (std::is_floating_point_v<typename T::value_type>) {
      result[i] = static_cast<typename T::value_type>(**arr.get(i)->as_floating_point());
    } else {
      result[i] = static_cast<typename T::value_type>(**arr.get(i)->as_integer());
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
  if (static_cast<std::size_t>(T::length()) != arr.size()) { return std::nullopt; }
  auto result = T{};
  for (auto i : std::views::iota(0, T::length())) {
    if constexpr (std::is_floating_point_v<typename T::value_type>) {
      if (const auto value = arr.get(i)->as_floating_point(); value != nullptr) {
        result[i] = static_cast<typename T::value_type>(value->get());
      } else {
        return std::nullopt;
      }
    } else {
      if (const auto value = arr.get(i)->as_integer(); value != nullptr) {
        result[i] = static_cast<typename T::value_type>(value->get());
      } else {
        return std::nullopt;
      }
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

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SERIALIZATION_H
