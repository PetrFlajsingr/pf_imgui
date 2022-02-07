/**
 * @file ui_tree_traversal.h
 * @brief Traversal of ui tree.
 * @author Petr Flajšingr
 * @date 2.11.20
 */
#ifndef PF_IMGUI_UI_TREE_TRAVERSAL_H
#define PF_IMGUI_UI_TREE_TRAVERSAL_H

#include <algorithm>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Layout.h>
#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {

namespace details {
/**
 * Quite an inefficient implementation due the massive use of dynamic_casts.
 * @param element root
 * @param callback
 */
inline void traverseImGuiTree_impl(Renderable &element, std::invocable<Renderable &> auto callback) {
  callback(element);
  if (auto ptrContainer = dynamic_cast<RenderablesContainer *>(&element); ptrContainer != nullptr) {
    std::ranges::for_each(ptrContainer->getRenderables(),
                          [&callback](const auto &child) { traverseImGuiTree_impl(*child, callback); });
  }
}
}  // namespace details

/**
 * Traverse UI tree in a BFS way. Call callback for each node.
 * @param root root of the UI tree
 * @param callback function to be called on each node
 */
PF_IMGUI_EXPORT inline void traverseImGuiTree(Renderable &root, std::invocable<Renderable &> auto callback) {
  details::traverseImGuiTree_impl(root, callback);
}

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_UI_TREE_TRAVERSAL_H
