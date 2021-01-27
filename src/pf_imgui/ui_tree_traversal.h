//
// Created by petr on 11/2/20.
//

#ifndef PF_IMGUI_TREE_TRAVERSAL_H
#define PF_IMGUI_TREE_TRAVERSAL_H

#include "elements/interface/ElementContainer.h"
#include "elements/interface/Renderable.h"
#include "src/pf_imgui/elements/interface/Layout.h"
#include <algorithm>

namespace pf::ui::ig {

namespace details {
inline void traverseImGuiTree_impl(Renderable &element, std::invocable<Renderable &> auto callback) {
  callback(element);
  if (auto ptrContainer = dynamic_cast<ElementContainer *>(&element); ptrContainer != nullptr) {
    std::ranges::for_each(ptrContainer->getChildren(),
                          [&callback](auto &child) { traverseImGuiTree_impl(child, callback); });
  } else if (auto ptrLayout = dynamic_cast<Layout *>(&element); ptrLayout != nullptr) {
    std::ranges::for_each(ptrLayout->getRenderables(),
                          [&callback](auto child) { traverseImGuiTree_impl(*child, callback); });
  }
}
}// namespace details

PF_IMGUI_EXPORT inline void traverseImGuiTree(Renderable &root, std::invocable<Renderable &> auto callback) {
  details::traverseImGuiTree_impl(root, callback);
}

}// namespace pf::ui::ig
#endif//PF_IMGUI_TREE_TRAVERSAL_H
