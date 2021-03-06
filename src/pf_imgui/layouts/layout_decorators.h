//
// Created by xflajs00 on 25.01.2021.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_DECORATORS_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_DECORATORS_H

#include <concepts>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Positionable.h>
#include <pf_imgui/interface/Resizable.h>
#include <utility>

namespace pf::ui::ig {

template<std::derived_from<Element> T>
class PF_IMGUI_EXPORT PositionDecorator : public T, public Positionable {
 public:
  template<typename... Args>
  requires std::constructible_from<T, Args...> explicit PositionDecorator(ImVec2 pos, Args &&...args)
      : T(std::forward<Args>(args)...), Positionable(pos) {}
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_DECORATORS_H
