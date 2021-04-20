/**
 * @file Bullet.h
 * @brief Decorator adding a bullet to an element.
 * @author Petr Flajšingr
 * @date 20.4.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_BULLET_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_BULLET_H

#include <pf_imgui/interface/Element.h>

namespace pf::ui::ig {

/**
 * @brief Wrapper for an element rendering a bullet point next to it.
 * @tparam T element rendered next to the bullet
 */
template<std::derived_from<Element> T>
class Bullet : public T {
 public:
  /**
   * Construct Bullet.
   * @tparam Args types of arguments for T's constructor
   * @param args arguments for T's constructor
   */
  template<typename... Args>
  explicit Bullet(Args &&...args) requires(std::constructible_from<T, Args...>) : T(std::forward<Args &&>(args)...) {}

 protected:
  void renderImpl() override {
    ImGui::Bullet();
    T::render();
  }

 private:
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_BULLET_H
