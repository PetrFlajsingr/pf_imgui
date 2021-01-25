//
// Created by xflajs00 on 25.01.2021.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_DECORATORS_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_DECORATORS_H

#include <concepts>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Element.h>
#include <pf_imgui/elements/interface/Resizable.h>
#include <pf_imgui/elements/interface/Positionable.h>

namespace pf::ui::ig {
template<typename T>
requires std::derived_from<T, Resizable> &&std::derived_from<T, Element> class PF_IMGUI_EXPORT PositionDecorator
    : public T, public Positionable {
 public:
  template<typename... Args>
  requires std::constructible_from<T, Args...> explicit PositionDecorator(ImVec2 pos, Args &&...args)
      : T(std::forward<Args>(args)...), Positionable(pos) {}
};

template<typename T>
requires std::derived_from<T, Resizable> &&std::derived_from<T, Element> class PF_IMGUI_EXPORT AnchorDecorator
    : public PositionDecorator<T> {
 public:
  template<typename... Args>
  requires std::constructible_from<T, Args...> explicit AnchorDecorator(Anchor anchor, Args &&...args)
      : T(std::forward<Args>(args)...), anchor(anchor) {}

  [[nodiscard]] Anchor getAnchor() const { return anchor; }

 private:
  Anchor anchor;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_DECORATORS_H
