/**
* @file Tree.h
* @brief Tree element.
* @author Petr Flajšingr
* @date 31.10.20
*/

#ifndef PF_IMGUI_ELEMENTS_TREE_H
#define PF_IMGUI_ELEMENTS_TREE_H

#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Collapsible.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/layouts/BoxLayout.h>
#include <string>

namespace pf::ui::ig {
enum class TreeNodeMarker { Bullet, Arrow };
enum class TreeType { Simple, Advanced };

class PF_IMGUI_EXPORT TreeLeaf;
template<TreeType treeType>
class PF_IMGUI_EXPORT TreeNode;
template<TreeType treeType>
class PF_IMGUI_EXPORT Tree;

// TODO: drag drop source/target
// TODO: limit selection - only one leaf active at a time, basically a selection group - maybe a generic class for groups of ValueObservable<bool>?
// TODO: comments
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
 TreeRecord(const std::string &elementName, const std::string &label, Flags<ImGuiTreeNodeFlags_> defaultFlags);

protected:
 Flags<ImGuiTreeNodeFlags_> flags{};
 TreeSelectionLimiter *limiter = nullptr;
};

template<TreeType treeType>
void traverseTree(TreeRecord &tree, std::invocable<details::TreeRecord &> auto callable);

}// namespace details

class PF_IMGUI_EXPORT TreeLeaf
   : public details::TreeRecord,
     public ValueObservable<bool>,
     public Savable,
     public StyleCustomizable<style::Style::FramePadding, style::Style::ItemSpacing, style::Style::FrameRounding>,
     public ColorCustomizable<style::ColorOf::Text, style::ColorOf::HeaderActive, style::ColorOf::HeaderHovered,
                              style::ColorOf::Header> {
public:
 TreeLeaf(const std::string &elementName, const std::string &label, bool selected = false,
          Persistent persistent = Persistent::No);

 void setValue(const bool &newValue) override;

protected:
 void renderImpl() override;
 void unserialize_impl(const toml::table &src) override;
 toml::table serialize_impl() override;
};

template<>
class PF_IMGUI_EXPORT TreeNode<TreeType::Simple>
   : public details::TreeRecord, public RenderablesContainer, public Collapsible {
public:
 TreeNode(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse = AllowCollapse::Yes,
          Persistent persistent = Persistent::No)
     : TreeNode(elementName, label, allowCollapse, persistent, Flags<ImGuiTreeNodeFlags_>{}) {}

 TreeNode<TreeType::Simple> &addNode(const std::string &name, const std::string &label,
                                     AllowCollapse allowCollapse = AllowCollapse::Yes) {
   auto &node = elementContainer.createChild<TreeNode<TreeType::Simple>>(
       name, label, allowCollapse, isPersistent() ? Persistent::Yes : Persistent::No);
   node.limiter = limiter;
   return node;
 }

 TreeLeaf &addLeaf(const std::string &name, const std::string &label, bool selected = false) {
   auto &leaf = elementContainer.createChild<TreeLeaf>(name, label, selected,
                                                       isPersistent() ? Persistent::Yes : Persistent::No);
   leaf.limiter = limiter;
   return leaf;
 }
 [[nodiscard]] std::vector<Renderable *> getRenderables() override { return elementContainer.getRenderables(); }

 void setNodeMarker(TreeNodeMarker marker) {
   switch (marker) {
     case TreeNodeMarker::Arrow: flags |= ImGuiTreeNodeFlags_Bullet; break;
     case TreeNodeMarker::Bullet: flags &= static_cast<ImGuiTreeNodeFlags_>(~ImGuiTreeNodeFlags_Bullet); break;
   }
 }

protected:
 TreeNode(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse, Persistent persistent,
          const Flags<ImGuiTreeNodeFlags_> &flags)
     : TreeRecord(elementName, label, flags), Collapsible(allowCollapse, persistent) {
   setCollapsed(true);
 }

 void renderImpl() override {
   auto colorStyle = setColorStack();
   auto style = setStyleStack();
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

template<>
class PF_IMGUI_EXPORT TreeNode<TreeType::Advanced>
   : public details::TreeRecord, public ElementContainer, public Collapsible {
public:
 TreeNode(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse = AllowCollapse::Yes,
          Persistent persistent = Persistent::No)
     : TreeNode(elementName, label, allowCollapse, persistent, Flags<ImGuiTreeNodeFlags_>{}) {}

 TreeNode<TreeType::Advanced> &addNode(const std::string &name, const std::string &label,
                                       AllowCollapse allowCollapse = AllowCollapse::Yes) {
   auto &node = createChild<TreeNode<TreeType::Advanced>>(name, label, allowCollapse,
                                                          isPersistent() ? Persistent::Yes : Persistent::No);
   node.limiter = limiter;
   return node;
 }

 TreeLeaf &addLeaf(const std::string &name, const std::string &label, bool selected = false) {
   auto &leaf = createChild<TreeLeaf>(name, label, selected, isPersistent() ? Persistent::Yes : Persistent::No);
   leaf.limiter = limiter;
   return leaf;
 }

 void setNodeMarker(TreeNodeMarker marker) {
   switch (marker) {
     case TreeNodeMarker::Arrow: flags |= ImGuiTreeNodeFlags_Bullet; break;
     case TreeNodeMarker::Bullet: flags &= static_cast<ImGuiTreeNodeFlags_>(~ImGuiTreeNodeFlags_Bullet); break;
   }
 }

protected:
 TreeNode(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse, Persistent persistent,
          const Flags<ImGuiTreeNodeFlags_> &flags)
     : TreeRecord(elementName, label, flags), Collapsible(allowCollapse, persistent) {
   setCollapsed(true);
 }

 void renderImpl() override {
   auto colorStyle = setColorStack();
   auto style = setStyleStack();
   setCollapsed(!ImGui::TreeNodeEx(getLabel().c_str(), *flags));
   RAII end{[this] {
     if (!isCollapsed() && !flags.is(ImGuiTreeNodeFlags_NoTreePushOnOpen)) { ImGui::TreePop(); }
   }};
   if (!isCollapsed()) {
     std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
   }
 }
};

template<TreeType treeType>
class PF_IMGUI_EXPORT TreeHeaderNode : public TreeNode<treeType> {
public:
 TreeHeaderNode(const std::string &elementName, const std::string &label,
                AllowCollapse allowCollapse = AllowCollapse::Yes, Persistent persistent = Persistent::No)
     : TreeNode<treeType>(elementName, label, allowCollapse, persistent, ImGuiTreeNodeFlags_CollapsingHeader) {}
};

template<TreeType treeType>
class PF_IMGUI_EXPORT Tree : public Element, public RenderablesContainer {
public:
 Tree(const std::string &name, const Size &size, Persistent persistent = Persistent::No)
     : Element(name), persistent(persistent), layout(name + "_layout", LayoutDirection::TopToBottom, size) {}

 TreeNode<treeType> &addNode(const std::string &name, const std::string &label,
                             AllowCollapse allowCollapse = AllowCollapse::Yes) {
   auto &node = layout.createChild<TreeNode<treeType>>(name, label, allowCollapse, persistent);
   node.limiter = &limiter;
   return node;
 }

 TreeHeaderNode<treeType> &addHeaderNode(const std::string &name, const std::string &label,
                                         AllowCollapse allowCollapse = AllowCollapse::Yes) {
   auto &node = layout.createChild<TreeHeaderNode<treeType>>(name, label, allowCollapse, persistent);
   node.limiter = &limiter;
   return node;
 }

 TreeLeaf &addLeaf(const std::string &name, const std::string &label, bool selected = false) {
   auto &leaf = layout.createChild<TreeLeaf>(name, label, selected, persistent);
   leaf.limiter = &limiter;
   return leaf;
 }

 [[nodiscard]] std::vector<Renderable *> getRenderables() override { return layout.getRenderables(); }

 void setLimitSelectionToOne(bool limit) {
   auto leafLimiter = limit ? &limiter : nullptr;
   traverse([leafLimiter](details::TreeRecord &record) {
     if (auto ptr = dynamic_cast<TreeLeaf *>(&record); ptr != nullptr) { ptr->limiter = leafLimiter; }
   });
 }

protected:
 void renderImpl() override { layout.render(); }

private:
 Persistent persistent;
 BoxLayout layout;
 details::TreeSelectionLimiter limiter;

 inline void traverse(std::invocable<details::TreeRecord &> auto fnc) {
   std::ranges::for_each(getRenderables(), [&](auto &renderable) {
     if (auto record = dynamic_cast<details::TreeRecord *>(renderable); record != nullptr) {
       details::traverseTree<treeType>(*record, fnc);
     }
   });
 }
};

template<TreeType treeType>
void details::traverseTree(details::TreeRecord &tree, std::invocable<details::TreeRecord &> auto callable) {
 callable(tree);
 if (auto node = dynamic_cast<TreeNode<treeType> *>(&tree); node != nullptr) {
   std::ranges::for_each(node->getRenderables(), [&](auto &renderable) {
     if (auto record = dynamic_cast<details::TreeRecord *>(renderable); record != nullptr) {
       callable(*record);
       if (auto node = dynamic_cast<TreeNode<treeType> *>(record); node != nullptr) {
         traverseTree<treeType>(*node, callable);
       }
     }
   });
 }
}

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_TREE_H
