/**
 * @file Tree.h
 * @brief Tree element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_TREE_H
#define PF_IMGUI_ELEMENTS_TREE_H

#include <iostream>
#include <memory>
#include <pf_common/Explicit.h>
#include <pf_common/Visitor.h>
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Collapsible.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/layouts/VerticalLayout.h>
#include <string>
#include <variant>
#include <vector>

namespace pf::ui::ig {
// TODO: simplify this
/**
 * @brief Type of marker rendered in tree nodes.
 */
enum class TreeNodeMarker { Bullet, Arrow };
/**
 * @brief Type of tree. Simple supports only other tree elements, Advanced allows to add any other element.
 */
enum class TreeType { Simple, Advanced };

class PF_IMGUI_EXPORT TreeLeaf;
template<TreeType treeType>
class PF_IMGUI_EXPORT TreeNode;
template<TreeType treeType>
class PF_IMGUI_EXPORT Tree;

namespace details {
struct PF_IMGUI_EXPORT TreeSelectionLimiter {
  TreeLeaf *selected = nullptr;
};

class PF_IMGUI_EXPORT TreeRecord : public ItemElement, public Labellable {
  friend class TreeNode<TreeType::Simple>;
  friend class TreeNode<TreeType::Advanced>;
  friend class Tree<TreeType::Simple>;
  friend class Tree<TreeType::Advanced>;

 public:
  TreeRecord(const std::string &elementName, const std::string &label, const Flags<ImGuiTreeNodeFlags_> &defaultFlags);

 protected:
  Flags<ImGuiTreeNodeFlags_> flags{};
  TreeSelectionLimiter *limiter = nullptr;
};
}  // namespace details

/**
 * @brief Leaf of Tree. Behaves as Selectable.
 */
class PF_IMGUI_EXPORT TreeLeaf : public details::TreeRecord, public ValueObservable<bool>, public Savable {
 public:
  /**
   * @brief Struct for construction of TreeLeaf.
   */
  struct Config {
    using Parent = TreeLeaf;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Label rendered on the element */
    bool selected = false;            /*!< Initial state */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct TreeLeaf
   * @param config construction args @see TreeLeaf::Config
   */
  explicit TreeLeaf(Config &&config);
  /**
   * Create TreeLeaf.
   * @param elementName unique name of the element
   * @param label label rendered on the element
   * @param selected
   * @param persistent enable/disable disk saving
   */
  TreeLeaf(const std::string &elementName, const std::string &label, bool selected = false,
           Persistent persistent = Persistent::No);

  void setValue(const bool &newValue) override;

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::HeaderActive, ColorOf::HeaderHovered, ColorOf::Header> color;
  StyleOptions<StyleOf::FramePadding, StyleOf::ItemSpacing, StyleOf::FrameRounding> style;
  Font font = Font::Default();

 protected:
  void renderImpl() override;
};

/**
 * @brief A Tree node containing other nodes or leaves.
 */
template<>
class PF_IMGUI_EXPORT TreeNode<TreeType::Simple>
    : public details::TreeRecord, public RenderablesContainer, public Collapsible {
 public:
  /**
   * @brief Struct for construction of TreeNode.
   */
  struct Config {
    using Parent = TreeNode;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered on the element */
    bool allowCollapse = true;        /*!< Allow collapse functionality */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct TreeNode
   * @param config construction args @see TreeNode::Config
   */
  explicit TreeNode(Config &&config)
      : TreeNode(std::string{config.name.value}, std::string{config.label.value},
                 config.allowCollapse ? AllowCollapse::Yes : AllowCollapse::No,
                 config.persistent ? Persistent::Yes : Persistent::No, Flags<ImGuiTreeNodeFlags_>{}) {}
  /**
   * Construct TreeNode.
   * @param elementName unique name of the element
   * @param label label rendered on the element
   * @param allowCollapse enable/disable user collapsing the node
   * @param persistent enable/disable disk saving
   */
  TreeNode(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse = AllowCollapse::Yes,
           Persistent persistent = Persistent::No)
      : TreeNode(elementName, label, allowCollapse, persistent, Flags<ImGuiTreeNodeFlags_>{}) {}

  /**
   * Create a new child node.
   * @param nodeName unique name of the element
   * @param label label rendered on the element
   * @param allowCollapse enable/disable user collapsing the node
   * @return reference to the newly created node
   */
  TreeNode<TreeType::Simple> &addNode(const std::string &nodeName, const std::string &label,
                                      AllowCollapse allowCollapse = AllowCollapse::Yes) {
    auto &node = elementContainer.createChild<TreeNode<TreeType::Simple>>(
        nodeName, label, allowCollapse, isPersistent() ? Persistent::Yes : Persistent::No);
    node.limiter = limiter;
    return node;
  }

  /**
   * Create a new leaf node.
   * @param leafName unique name of the element
   * @param label label rendered on the element
   * @param selected
   * @return reference to the newly created leaf
   */
  TreeLeaf &addLeaf(const std::string &leafName, const std::string &label, bool selected = false) {
    auto &leaf = elementContainer.createChild<TreeLeaf>(leafName, label, selected,
                                                        isPersistent() ? Persistent::Yes : Persistent::No);
    leaf.limiter = limiter;
    return leaf;
  }
  [[nodiscard]] std::vector<Renderable *> getRenderables() override { return elementContainer.getRenderables(); }

  [[nodiscard]] auto getTreeNodes() {
    return elementContainer.getChildren() | ranges::views::transform([](auto &child) -> details::TreeRecord & {
             return dynamic_cast<details::TreeRecord &>(child);
           });
  }

  /**
   * Set what kind of marker is being rendered on the left part of the node.
   * @param marker
   */
  void setNodeMarker(TreeNodeMarker marker) {
    switch (marker) {
      case TreeNodeMarker::Arrow: flags |= ImGuiTreeNodeFlags_Bullet; break;
      case TreeNodeMarker::Bullet: flags &= static_cast<ImGuiTreeNodeFlags_>(~ImGuiTreeNodeFlags_Bullet); break;
    }
  }

 protected:
  TreeNode(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse, Persistent persistent,
           const Flags<ImGuiTreeNodeFlags_> &flags)
      : TreeRecord(elementName, label, flags), Collapsible(allowCollapse, true, persistent) {}

  void renderImpl() override {
    [[maybe_unused]] auto colorScoped = color.applyScoped();
    [[maybe_unused]] auto styleScoped = style.applyScoped();
    [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
    ImGui::SetNextItemOpen(!isCollapsed());
    setCollapsed(!ImGui::TreeNodeEx(getLabel().c_str(), *flags));
    RAII end{[this] {
      if (!isCollapsed() && !flags.is(ImGuiTreeNodeFlags_NoTreePushOnOpen)) { ImGui::TreePop(); }
    }};
    if (!isCollapsed()) {
      std::ranges::for_each(elementContainer.getChildren(), [](auto &child) { child.render(); });
    }
  }

 private:
  ElementContainer elementContainer;
};

/**
 * @brief A Tree node containing other nodes or leaves.
 */
template<>
class PF_IMGUI_EXPORT TreeNode<TreeType::Advanced>
    : public details::TreeRecord, public ElementContainer, public Collapsible {
 public:
  /**
   * @brief Struct for construction of TreeNode.
   */
  struct Config {
    using Parent = TreeNode;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered on the element */
    bool allowCollapse = true;        /*!< Allow collapse functionality */
    bool persistent = false;          /*!< Allow state saving to disk */
  };

  /**
   * Construct TreeNode
   * @param config construction args @see TreeNode::Config
   */
  explicit TreeNode(Config &&config)
      : TreeNode(std::string{config.name.value}, std::string{config.label.value},
                 config.allowCollapse ? AllowCollapse::Yes : AllowCollapse::No,
                 config.persistent ? Persistent::Yes : Persistent::No, Flags<ImGuiTreeNodeFlags_>{}) {}
  /**
   * Construct TreeNode.
   * @param elementName unique name of the element
   * @param label label rendered on the element
   * @param allowCollapse enable/disable user collapsing the node
   * @param persistent enable/disable disk saving
   */
  TreeNode(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse = AllowCollapse::Yes,
           Persistent persistent = Persistent::No)
      : TreeNode(elementName, label, allowCollapse, persistent, Flags<ImGuiTreeNodeFlags_>{}) {}

  /**
   * Create a new child node.
   * @param nodeName unique name of the element
   * @param label label rendered on the element
   * @param allowCollapse enable/disable user collapsing the node
   * @return reference to the newly created node
   */
  TreeNode<TreeType::Advanced> &addNode(const std::string &nodeName, const std::string &label,
                                        AllowCollapse allowCollapse = AllowCollapse::Yes) {
    auto &node = createChild<TreeNode<TreeType::Advanced>>(nodeName, label, allowCollapse,
                                                           isPersistent() ? Persistent::Yes : Persistent::No);
    node.limiter = limiter;
    return node;
  }

  /**
   * Create a new leaf node.
   * @param leafName unique name of the element
   * @param label label rendered on the element
   * @param selected
   * @return reference to the newly created leaf
   */
  TreeLeaf &addLeaf(const std::string &leafName, const std::string &label, bool selected = false) {
    auto &leaf = createChild<TreeLeaf>(leafName, label, selected, isPersistent() ? Persistent::Yes : Persistent::No);
    leaf.limiter = limiter;
    return leaf;
  }

  /**
   * Set what kind of marker is being rendered on the left part of the node.
   * @param marker
   */
  void setNodeMarker(TreeNodeMarker marker) {
    switch (marker) {
      case TreeNodeMarker::Arrow: flags |= ImGuiTreeNodeFlags_Bullet; break;
      case TreeNodeMarker::Bullet: flags &= static_cast<ImGuiTreeNodeFlags_>(~ImGuiTreeNodeFlags_Bullet); break;
    }
  }

  [[nodiscard]] auto getTreeNodes() {
    return getChildren() | ranges::views::transform([](auto &child) -> details::TreeRecord & {
             return dynamic_cast<details::TreeRecord &>(child);
           });
  }

 protected:
  TreeNode(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse, Persistent persistent,
           const Flags<ImGuiTreeNodeFlags_> &flags)
      : TreeRecord(elementName, label, flags), Collapsible(allowCollapse, true, persistent) {}

  void renderImpl() override {
    [[maybe_unused]] auto colorScoped = color.applyScoped();
    [[maybe_unused]] auto styleScoped = style.applyScoped();
    [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
    ImGui::SetNextItemOpen(!isCollapsed());
    setCollapsed(!ImGui::TreeNodeEx(getLabel().c_str(), *flags));
    RAII end{[this] {
      if (!isCollapsed() && !flags.is(ImGuiTreeNodeFlags_NoTreePushOnOpen)) { ImGui::TreePop(); }
    }};
    if (!isCollapsed()) {
      std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
    }
  }
};

/**
 * @brief A Tree node which has a distinct background and doesn't offset its child nodes to the right.
 * @tparam treeType
 */
template<TreeType treeType>
class PF_IMGUI_EXPORT TreeHeaderNode : public TreeNode<treeType> {
 public:
  /**
   * @brief Struct for construction of TreeHeaderNode.
   */
  struct Config {
    using Parent = TreeHeaderNode;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered on the element */
    bool allowCollapse = true;        /*!< Allow collapse functionality */
    bool persistent = false;          /*!< Allow state saving to disk */
  };

  /**
   * Construct TreeHeaderNode
   * @param config construction args @see TreeHeaderNode::Config
   */
  explicit TreeHeaderNode(Config &&config)
      : TreeNode<treeType>(std::string{config.name.value}, std::string{config.label.value},
                           config.allowCollapse ? AllowCollapse::Yes : AllowCollapse::No,
                           config.persistent ? Persistent::Yes : Persistent::No, ImGuiTreeNodeFlags_CollapsingHeader) {}
  /**
   * Construct TreeHeaderNode.
   * @param elementName unique name of the element
   * @param label label rendered on the element
   * @param allowCollapse enable/disable user collapsing the node
   * @param persistent enable/disable disk saving
   */
  TreeHeaderNode(const std::string &elementName, const std::string &label,
                 AllowCollapse allowCollapse = AllowCollapse::Yes, Persistent persistent = Persistent::No)
      : TreeNode<treeType>(elementName, label, allowCollapse, persistent, ImGuiTreeNodeFlags_CollapsingHeader) {}
};

/**
 * @brief A structure containing tree nodes and leaves.
 * @tparam treeType
 */
template<TreeType treeType>
class PF_IMGUI_EXPORT Tree : public ElementWithID, public RenderablesContainer {
 public:
  /**
   * @brief Struct for construction of Tree.
   */
  struct Config {
    using Parent = Tree;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    bool showBorder = false;         /*!< Enable rendering of Tree area border */
    bool persistent = false;         /*!< Allow state saving to disk */
  };
  /**
   * Construct Tree
   * @param config construction args @see Tree::Config
   */
  explicit Tree(Config &&config)
      : ElementWithID(std::string{config.name.value}), persistent(config.persistent ? Persistent::Yes : Persistent::No),
        layout({.name = "layout",
                .size = Size::Auto(),
                .showBorder = config.showBorder ? ShowBorder::Yes : ShowBorder::No}) {}
  /**
   * Construct tree.
   * @param name unique name of the element
   * @param size size of the element
   * @param showBorder render a border around the tree area
   * @param persistent enable/disable disk saving
   */
  explicit Tree(const std::string &name, ShowBorder showBorder = ShowBorder::No, Persistent persistent = Persistent::No)
      : ElementWithID(name), persistent(persistent),
        layout({.name = "layout", .size = Size::Auto(), .showBorder = showBorder == ShowBorder::Yes ? true : false}) {}

  /**
   * Create a new child node.
   * @param name unique name of the element
   * @param label label rendered on the element
   * @param allowCollapse enable/disable user collapsing the node
   * @return reference to the newly created node
   */
  TreeNode<treeType> &addNode(const std::string &name, const std::string &label,
                              AllowCollapse allowCollapse = AllowCollapse::Yes) {
    auto &node = layout.createChild<TreeNode<treeType>>(name, label, allowCollapse, persistent);
    node.limiter = limiter.get();
    return node;
  }

  /**
   * Create a new child header node.
   * @param nodeName unique name of the element
   * @param label label rendered on the element
   * @param allowCollapse enable/disable user collapsing the node
   * @return reference to the newly created node
   */
  TreeHeaderNode<treeType> &addHeaderNode(const std::string &nodeName, const std::string &label,
                                          AllowCollapse allowCollapse = AllowCollapse::Yes) {
    auto &node = layout.createChild<TreeHeaderNode<treeType>>(nodeName, label, allowCollapse, persistent);
    node.limiter = limiter.get();
    return node;
  }

  /**
   * Create a new leaf node.
   * @param leafName unique name of the element
   * @param label label rendered on the element
   * @param selected
   * @return reference to the newly created leaf
   */
  TreeLeaf &addLeaf(const std::string &leafName, const std::string &label, bool selected = false) {
    auto &leaf = layout.createChild<TreeLeaf>(leafName, label, selected, persistent);
    leaf.limiter = limiter.get();
    return leaf;
  }

  [[nodiscard]] std::vector<Renderable *> getRenderables() override { return layout.getRenderables(); }

  [[nodiscard]] auto getTreeNodes() {
    return layout.getChildren() | ranges::views::transform([](auto &child) -> details::TreeRecord & {
             return dynamic_cast<details::TreeRecord &>(child);
           });
  }

  /**
   * Enable/disable limiting leaf nodes to have only one selected.
   * @param limit
   */
  void setLimitSelectionToOne(bool limit) {
    if (limit && limiter == nullptr) {
      limiter = std::make_unique<details::TreeSelectionLimiter>();
    } else if (!limit) {
      limiter = nullptr;
    }
    auto leafLimiter = limiter.get();
    traversePreOrder(Visitor{[&](TreeLeaf *leaf, auto) {
                               leaf->limiter = leafLimiter;
                               return true;
                             },
                             [](auto, auto) { return true; }});
  }

  // node, depth
  // return true to continue deeper into the tree, false if not
  // in order
  template<typename F>
    requires(std::invocable<F, TreeLeaf *, std::size_t> && std::invocable<F, TreeNode<treeType> *, std::size_t>)
  void traversePreOrder(F &&callable) {
    std::ranges::for_each(layout.getChildren() | ranges::views::transform([](auto &child) -> details::TreeRecord & {
                            return dynamic_cast<details::TreeRecord &>(child);
                          }),
                          [&](auto &record) { traversePreOrderImpl(record, callable, 0); });
  }

  template<typename F>
    requires(std::invocable<F, TreeLeaf *, std::size_t> && std::invocable<F, TreeNode<treeType> *, std::size_t>)
  void traversePostOrder(F &&callable) {
    std::ranges::for_each(layout.getChildren() | ranges::views::transform([](auto &child) -> details::TreeRecord & {
                            return dynamic_cast<details::TreeRecord &>(child);
                          }),
                          [&](auto &record) { traversePosOrderImpl(record, callable, 0); });
  }

 protected:
  void renderImpl() override {
    [[maybe_unused]] auto colorScoped = color.applyScoped();
    [[maybe_unused]] auto styleScoped = style.applyScoped();
    [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
    layout.render();
  }

 private:
  Persistent persistent;
  VerticalLayout layout;
  std::unique_ptr<details::TreeSelectionLimiter> limiter = nullptr;

  template<typename F>
    requires(std::invocable<F, TreeLeaf *, std::size_t> && std::invocable<F, TreeNode<treeType> *, std::size_t>)
  void traversePreOrderImpl(details::TreeRecord &node, F &&callable, std::size_t depth) {
    if (auto nodePtr = dynamic_cast<TreeNode<treeType> *>(&node); nodePtr != nullptr) {
      if (!callable(nodePtr, depth)) { return; }
      std::ranges::for_each(nodePtr->getTreeNodes(),
                            [&](auto &record) { traversePreOrderImpl(record, callable, depth + 1); });
    } else if (auto leafPtr = dynamic_cast<TreeLeaf *>(&node); leafPtr != nullptr) {
      callable(leafPtr, depth);
    }
  }
  template<typename F>
    requires(std::invocable<F, TreeLeaf *, std::size_t> && std::invocable<F, TreeNode<treeType> *, std::size_t>)
  void traversePosOrderImpl(details::TreeRecord &node, F &&callable, std::size_t depth) {
    if (auto nodePtr = dynamic_cast<TreeNode<treeType> *>(&node); nodePtr != nullptr) {
      std::ranges::for_each(nodePtr->getTreeNodes(),
                            [&](auto &record) { traversePosOrderImpl(record, callable, depth + 1); });
      callable(nodePtr, depth);
    } else if (auto leafPtr = dynamic_cast<TreeLeaf *>(&node); leafPtr != nullptr) {
      callable(leafPtr, depth);
    }
  }
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_TREE_H
