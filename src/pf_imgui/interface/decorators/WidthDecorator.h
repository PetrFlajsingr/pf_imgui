//
// Created by petr on 5/2/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_DECORATORS_RESIZABLEDECORATOR_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_DECORATORS_RESIZABLEDECORATOR_H

#include <imgui.h>
#include <pf_common/exceptions/StackTraceException.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {

// (so -FLT_MIN always align width to the right side).
template<std::derived_from<ItemElement> T>
requires(!std::derived_from<T, Resizable>) class WidthDecorator : public T {
 public:
  /**
   * Construct WidthDecorator.
   * @tparam Args argument types for T's constructor
   * @param elementWidth width of the element, >0.0f: width in pixels, <0.0f align xx pixels to the right of window
   * @param args arguments for T's constructor
   */
  template<typename... Args>
  requires std::constructible_from<T, Args...>
  explicit WidthDecorator(float elementWidth, Args &&...args)
      : T(std::forward<Args>(args)...), Positionable(pos), width(elementWidth) {}

  /**
   * Get current element width.
   * @return current element width.
   */
  [[nodiscard]] float getWidth() const { return width; }
  void setWidth(float newWidth) { width = newWidth; }

 protected:
  void renderImpl() override {
    ImGui::SetNextItemWidth(width);
    T::renderImpl();
  }

 private:
  float width{};
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_DECORATORS_RESIZABLEDECORATOR_H
