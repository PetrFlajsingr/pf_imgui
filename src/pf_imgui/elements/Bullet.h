/**
 * @file Bullet.h
 * @brief Decorator adding a bullet to an element.
 * @author Petr Flajšingr
 * @date 20.4.21
 */

#ifndef PF_IMGUI_ELEMENTS_BULLET_H
#define PF_IMGUI_ELEMENTS_BULLET_H

#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Wrapper for an element rendering a bullet point next to it.
 * @tparam T element rendered next to the bulletW
 */
template<std::derived_from<Element> T>
class PF_IMGUI_EXPORT Bullet : public T {
 public:
  /**
   * @brief Struct for construction of Bullet
   */
  struct Config {
    using Parent = Bullet;
    Explicit<typename T::Config> config; /*!< Config of the underlying Element */
    explicit(false) operator typename T::Config() { return config; }
  };
  /**
   * Construct Bullet.
   * @tparam Args types of arguments for T's constructor
   * @param args arguments for T's constructor
   */
  template<typename... Args>
  explicit Bullet(Args &&...args)
    requires(std::constructible_from<T, Args...>);

 protected:
  void renderImpl() override;
};

template<std::derived_from<Element> T>
template<typename... Args>
Bullet<T>::Bullet(Args &&...args)
  requires(std::constructible_from<T, Args...>)
    : T(std::forward<Args>(args)...) {}

template<std::derived_from<Element> T>
void Bullet<T>::renderImpl() {
  ImGui::Bullet();
  T::renderImpl();
}
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_BULLET_H
