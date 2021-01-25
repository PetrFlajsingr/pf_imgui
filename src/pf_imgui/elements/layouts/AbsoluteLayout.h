//
// Created by xflajs00 on 25.01.2021.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
#include "Layout.h"
#include <concepts>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Element.h>
#include <pf_imgui/elements/interface/Resizable.h>

namespace pf::ui::ig {

template<typename T>
requires std::derived_from<T, Resizable> &&std::derived_from<T, Element> class PositionDecorator : public T {
 public:
  template<typename... Args>
  requires std::constructible_from<T, Args...> PositionDecorator(ImVec2 pos, Args &&...args)
      : T(std::forward<Args>(args)...), position(pos) {}

  [[nodiscard]] ImVec2 getPosition() const { return position; }
  void setPosition(ImVec2 pos) { position = pos; }

 private:
  ImVec2 position;
};
// TODO: implement
class PF_IMGUI_EXPORT AbsoluteLayout : public Layout {
 public:
 private:
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
