/**
 * @file PositionDecorator.h
 * @brief Decorator for non-positionable elements.
 * @author Petr Flajšingr
 * @date 25.1.21
 */

#ifndef PF_IMGUI_LAYOUTS_LAYOUT_DECORATORS_H
#define PF_IMGUI_LAYOUTS_LAYOUT_DECORATORS_H

#include <concepts>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Positionable.h>
#include <pf_imgui/interface/Resizable.h>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Decorator for non-Positionable elements.
 *
 * This decorator adds Positionable capabilities to any Element. It is used for AbsoluteLayout.
 *
 * @todo: custom position struct
 */
template<std::derived_from<Element> T>
class PF_IMGUI_EXPORT PositionDecorator : public T, public Positionable {
 public:
  /**
   * Construct PositionDecorator.
   * @tparam Args argument types for T's constructor
   * @param pos starting position
   * @param args arguments for T's constructor
   */
  template<typename... Args>
  requires std::constructible_from<T, Args...>
  explicit PositionDecorator(ImVec2 pos, Args &&...args) : T(std::forward<Args>(args)...), Positionable(pos) {}
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_LAYOUTS_LAYOUT_DECORATORS_H