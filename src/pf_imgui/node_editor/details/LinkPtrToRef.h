//
// Created by xflajs00 on 02.04.2022.
//

#ifndef PF_IMGUI_LINKPTRTOREF_H
#define PF_IMGUI_LINKPTRTOREF_H

#include <concepts>
#include <pf_imgui/node_editor/Link.h>

namespace pf::ui::ig::details {
struct LinkPtrToRef {
  inline Link &operator()(const auto &linkPtr) { return *linkPtr; }
};
struct LinkPtrToConstRef {
  inline const Link &operator()(const auto &linkPtr) { return *linkPtr; }
};
}  // namespace pf::ui::ig::details

#endif  //PF_IMGUI_LINKPTRTOREF_H
