/**
 * @file ElementBase.h
 * @brief Base class for objects which can be considered UI elements.
 * @author Petr Flajšingr
 * @date 17.6.22
 */

#ifndef PF_IMGUI_INTERFACE_ELEMENT_BASE_H
#define PF_IMGUI_INTERFACE_ELEMENT_BASE_H

#include <pf_imgui/interface/Observable_impl.h>
#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {

/**
 * @brief Base class for anythin which can be considered a UI element.
 *
 * @attention For classes needing ids/fonts use Element
 * @attention For classes with hover/focus/tooltip etc. use ItemElement
 */
class PF_IMGUI_EXPORT ElementBase : public Renderable {
 public:
  /**
   * Construct ElementBase.
   * @param name ID of the element.
   */
  ElementBase(const std::string &name);
  ~ElementBase() override;
  ElementBase(ElementBase &&other) noexcept;
  ElementBase &operator=(ElementBase &&other) noexcept;

  /**
   * Add a listener called when this object is destroyed.
   * Most of the object will be destroyed at this point.
   *
   * @param listener listener to call
   * @return Subscription to allow for listener cancellation
   */
  Subscription addDestroyListener(std::invocable auto &&listener) {
    return observableDestroy.addListener(std::forward<decltype(listener)>(listener));
  }

 private:
  Observable_impl<> observableDestroy;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_INTERFACE_ELEMENT_BASE_H
