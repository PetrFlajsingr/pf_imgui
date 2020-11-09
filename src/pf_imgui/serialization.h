//
// Created by petr on 11/2/20.
//

#ifndef PF_IMGUI_SERIALIZATION_H
#define PF_IMGUI_SERIALIZATION_H

#include "elements.h"
#include "ui_tree_traversal.h"
#include <toml++/toml_array.hpp>
#include <toml++/toml_table.hpp>

namespace pf::ui::ig {

PF_IMGUI_EXPORT inline toml::table serializeImGuiTree(Element &root) {
  auto result = toml::table();
  traverseImGuiTree(root, [&result](Element &element) {
    if (auto ptrSavable = dynamic_cast<SavableElement *>(&element); ptrSavable != nullptr) {
      const auto optSerialised = ptrSavable->serialize();
      if (optSerialised.has_value()) {
        result.insert_or_assign(ptrSavable->getName(), *optSerialised);
      }
    }
  });
  return result;
}

template<typename T>
PF_IMGUI_EXPORT T deserializeGlmVec(const toml::array &arr) {
  auto result = T{};
  for (auto i : std::views::iota(0, T::length())) {
    if constexpr (std::same_as<typename T::value_type, float>) {
      result[i] = **arr.get(i)->as_floating_point();
    } else {
      result[i] = **arr.get(i)->as_integer();
    }
  }
  return result;
}
template<typename T>
PF_IMGUI_EXPORT toml::array serializeGlmVec(const T &vec) {
  auto result = toml::array{};
  for (auto i : std::views::iota(0, T::length())) { result.push_back(vec[i]); }
  return result;
}

}// namespace pf::ui::ig

#endif//PF_IMGUI_SERIALIZATION_H
