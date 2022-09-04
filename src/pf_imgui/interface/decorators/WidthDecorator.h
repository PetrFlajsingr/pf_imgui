/**
 * @file WidthDecorator.h
 * @brief Decorator to enable control of element width.
 * @author Petr Flajšingr
 * @date 2.5.21
 */

#ifndef PF_IMGUI_INTERFACE_DECORATORS_WIDTHDECORATOR_H
#define PF_IMGUI_INTERFACE_DECORATORS_WIDTHDECORATOR_H

#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/Size.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/reactive/Observable.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Decorator to change element width.
 *
 */
template<std::derived_from<ItemElement> T>
class PF_IMGUI_EXPORT WidthDecorator : public T {
 public:
  struct Config {
    using Parent = WidthDecorator;
    Explicit<Width> width;
    typename T::Config config;
    operator typename T::Config() { return config; }
  };
  /**
   * Construct WidthDecorator.
   * @tparam Args argument types for T's constructor
   * @param elementWidth width of the element, >0.0f: width in pixels, <0.0f align xx pixels to the right of window - -FLT_MAX aligns to the right
   * @param args arguments for T's constructor
   */
  template<typename... Args>
    requires std::constructible_from<T, Args...>
  explicit WidthDecorator(Width elementWidth, Args &&...args) : T(std::forward<Args>(args)...), width(elementWidth) {}

  explicit WidthDecorator(Config &&config) : T(std::move(config.config)), width(config.width) {}

  Observable<Width> width;

 protected:
  void renderImpl() override {
    ImGui::SetNextItemWidth(static_cast<float>(*width));
    T::renderImpl();
  }
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_DECORATORS_WIDTHDECORATOR_H
