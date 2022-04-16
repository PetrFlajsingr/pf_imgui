//
// Created by xflajs00 on 15.04.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_COMMON_H
#define PF_IMGUI_BLUEPRINT_COMMON_H

#include <functional>
#include <memory>
#include <optional>
#include <static_type_info.h>
#include <toml++/toml.h>
#include <type_traits>
#include <unordered_map>

namespace pf::ui::ig {
class NodeEditor;
class Node;
namespace bp {
using NodeTypeIdentifier = std::string;
using PinTypeIdentifier = std::string;
class Node;
class Pin;
}  // namespace bp
}  // namespace pf::ui::ig
// TODO: move this to pf_common
namespace pf::ui::ig::bp::details {
template<typename T>
[[nodiscard]] constexpr std::string_view RawTypeName() {
#ifndef _MSC_VER
  return __PRETTY_FUNCTION__;
#else
  return __FUNCSIG__;
#endif
}

struct TypeNameFormat {
  std::size_t junk_leading = 0;
  std::size_t junk_total = 0;
};

constexpr TypeNameFormat type_name_format = [] {
  TypeNameFormat ret;
  std::string_view sample = RawTypeName<int>();
  ret.junk_leading = sample.find("int");
  ret.junk_total = sample.size() - 3;
  return ret;
}();
static_assert(type_name_format.junk_leading != std::size_t(-1),
              "Unable to determine the type name format on this compiler.");

template<typename T>
static constexpr auto type_name_storage = [] {
  std::array<char, RawTypeName<T>().size() - type_name_format.junk_total + 1> ret{};
  std::copy_n(RawTypeName<T>().data() + type_name_format.junk_leading, ret.size() - 1, ret.data());
  return ret;
}();

template<typename T>
[[nodiscard]] constexpr std::string_view TypeName() {
  return {type_name_storage<T>.data(), type_name_storage<T>.size() - 1};
}

template<typename T>
[[nodiscard]] constexpr const char *TypeNameCstr() {
  return type_name_storage<T>.data();
}
}  // namespace pf::ui::ig::bp::details

#define PF_IMGUI_BLUEPRINT_NODE_ID(type)                                                                               \
  [[nodiscard]] static inline NodeTypeIdentifier UniqueNodeIdentifier() { return details::TypeNameCstr<type>(); }      \
  [[nodiscard]] inline NodeTypeIdentifier getNodeTypeId() const override { return UniqueNodeIdentifier(); }
#define PF_IMGUI_BLUEPRINT_PIN_ID(type)                                                                                \
  [[nodiscard]] static inline PinTypeIdentifier UniquePinIdentifier() { return details::TypeNameCstr<type>(); }        \
  [[nodiscard]] inline PinTypeIdentifier getPinTypeId() const override { return UniquePinIdentifier(); }

namespace pf::ui::ig::bp {
namespace details {

template<typename T>
concept TomlConstructiblePin =
    std::derived_from<T, pf::ui::ig::bp::Pin> && requires(ig::Node *parent, const toml::table &src) {
                                                   {
                                                     T::ConstructFromToml(parent, src)
                                                     } -> std::same_as<std::unique_ptr<T>>;
                                                   { T::UniquePinIdentifier() } -> std::same_as<NodeTypeIdentifier>;
                                                 };
template<typename T>
concept TomlConstructibleNode =
    std::derived_from<T, pf::ui::ig::bp::Node> && requires(ig::NodeEditor *parent, const toml::table &src) {
                                                    {
                                                      T::ConstructFromToml(parent, src)
                                                      } -> std::same_as<std::unique_ptr<T>>;
                                                    { T::UniqueNodeIdentifier() } -> std::same_as<PinTypeIdentifier>;
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
    const auto typeId = PinType::UniquePinIdentifier();
    pinConstructors[typeId] = &PinType::ConstructFromToml;
  }
  template<details::TomlConstructibleNode NodeType>
  void registerConstruction() {
    const auto typeId = NodeType::UniqueNodeIdentifier();
    nodeConstructors[typeId] = &NodeType::ConstructFromToml;
  }

  [[nodiscard]] std::optional<std::unique_ptr<Pin>> constructPin(PinTypeIdentifier typeId, ig::Node *parent,
                                                                 const toml::table &src);
  [[nodiscard]] std::optional<std::unique_ptr<Node>> constructNode(NodeTypeIdentifier typeId, ig::NodeEditor *parent,
                                                                   const toml::table &src);

 private:
  static inline std::unique_ptr<NodeEditorLoading> instance = nullptr;
  std::unordered_map<PinTypeIdentifier, std::function<std::unique_ptr<Pin>(ig::Node *, const toml::table &)>>
      pinConstructors;
  std::unordered_map<NodeTypeIdentifier, std::function<std::unique_ptr<Node>(ig::NodeEditor *, const toml::table &)>>
      nodeConstructors;
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_COMMON_H
