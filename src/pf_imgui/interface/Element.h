/**
 * @file Element.h
 * @brief Base class for objects which can be considered UI elements.
 * @author Petr Flajšingr
 * @date 17.6.22
 */

#ifndef PF_IMGUI_INTERFACE_ELEMENT_H
#define PF_IMGUI_INTERFACE_ELEMENT_H

#include <pf_imgui/reactive/Observable_impl.h>
#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {

/**
 * @brief Base class for anythin which can be considered a UI element.
 *
 * @attention For classes needing ids/fonts use ElementWithID
 * @attention For classes with hover/focus/tooltip etc. use ItemElement
 */
class PF_IMGUI_EXPORT Element : public Renderable {
 public:
  /**
   * Construct Element.
   * @param elementName ID of the element.
   */
  explicit Element(const std::string &elementName);
  ~Element() override;
  Element(Element &&other) noexcept;
  Element &operator=(Element &&other) noexcept;

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

#endif  //PF_IMGUI_INTERFACE_ELEMENT_H
