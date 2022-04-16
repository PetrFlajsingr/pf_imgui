//
// Created by xflajs00 on 15.04.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_COMMON_H
#define PF_IMGUI_BLUEPRINT_COMMON_H

#include <pf_imgui/node_editor/blueprint/nodes/Node.h>
#include <pf_imgui/node_editor/blueprint/pins/Pin.h>
#include <static_type_info.h>
#include <type_traits>

#define PF_IMGUI_BLUEPRINT_OVERRIDE_GETTYPEID(type)                                                                    \
  [[nodiscard]] inline static_type_info::TypeIndex getTypeId() const override {                                        \
    return static_type_info::getTypeIndex<type>();                                                                     \
  }

namespace pf::ui::ig::bp {
namespace details {

inline std::intptr_t typeIdToIntPtr_t(static_type_info::TypeIndex typeId) {
  return reinterpret_cast<std::intptr_t>(reinterpret_cast<const void *>(typeId));
}

template<typename T>
concept TomlConstructiblePin = std::derived_from<T, Pin> && requires(const toml::table &src) {
                                                              {
                                                                T::ConstructFromToml(src)
                                                                } -> std::same_as<std::unique_ptr<T>>;
                                                            };
template<typename T>
concept TomlConstructibleNode = std::derived_from<T, Node> && requires(const toml::table &src) {
                                                                {
                                                                  T::ConstructFromToml(src)
                                                                  } -> std::same_as<std::unique_ptr<T>>;
                                                              };

}  // namespace details

class NodeEditorLoading {
  NodeEditorLoading() = default;

 public:
  NodeEditorLoading(const NodeEditorLoading &) = delete;
  NodeEditorLoading &operator=(const NodeEditorLoading &) = delete;
  NodeEditorLoading(NodeEditorLoading &&) = delete;
  NodeEditorLoading &operator=(NodeEditorLoading &&) = delete;

  [[nodiscard]] inline static NodeEditorLoading *Get() {
    if (instance == nullptr) { instance = std::unique_ptr<NodeEditorLoading>(new NodeEditorLoading{}); }
    return instance.get();
  }

  template<details::TomlConstructiblePin PinType>
  void registerConstruction() {
    const auto typeIndex = reinterpret_cast<std::intptr_t>(static_type_info::getTypeIndex<PinType>());
    pinConstructors[typeIndex] = &PinType::ConstructFromToml;
  }
  template<details::TomlConstructibleNode NodeType>
  void registerConstruction() {
    const auto typeIndex = reinterpret_cast<std::intptr_t>(static_type_info::getTypeIndex<NodeType>());
    nodeConstructors[typeIndex] = &NodeType::ConstructFromToml;
  }

  [[nodiscard]] std::optional<std::unique_ptr<Pin>> constructPin(std::intptr_t typeId, const toml::table &src) {
    if (auto iter = pinConstructors.find(typeId); iter != pinConstructors.end()) { return iter->second(src); }
    return std::nullopt;
  }
  [[nodiscard]] std::optional<std::unique_ptr<Node>> constructNode(std::intptr_t typeId, const toml::table &src) {
    if (auto iter = nodeConstructors.find(typeId); iter != nodeConstructors.end()) { return iter->second(src); }
    return std::nullopt;
  }

 private:
  static inline std::unique_ptr<NodeEditorLoading> instance = nullptr;
  std::unordered_map<std::intptr_t, std::function<std::unique_ptr<Pin>(const toml::table &)>> pinConstructors;
  std::unordered_map<std::intptr_t, std::function<std::unique_ptr<Node>(const toml::table &)>> nodeConstructors;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_COMMON_H
