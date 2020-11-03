//
// Created by petr on 11/2/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_UI_TREE_TRAVERSAL_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_UI_TREE_TRAVERSAL_H

#include "elements.h"
#include <algorithm>

namespace pf::ui::ig {

namespace details {
inline void traverseImGuiTree_impl(Element &element,
                                   std::invocable<Element &> auto callback) {
  callback(element);
  if (auto ptrContainer = dynamic_cast<Container *>(&element); ptrContainer != nullptr) {
    std::ranges::for_each(ptrContainer->getChildren(), [&callback](auto &child) {
      traverseImGuiTree_impl(child.get(), callback);
    });
  }
}
}// namespace details

inline void traverseImGuiTree(Element &root, std::invocable<Element &> auto callback) {
  details::traverseImGuiTree_impl(root, callback);
}

}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_UI_TREE_TRAVERSAL_H
