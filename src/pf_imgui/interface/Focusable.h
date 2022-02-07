/**
 * @file Focusable.h
 * @brief Interface for elements which can hold focus.
 * @author Petr Flajšingr
 * @date 23.1.21
 */

#ifndef PF_IMGUI_INTERFACE_FOCUSABLE_H
#define PF_IMGUI_INTERFACE_FOCUSABLE_H

#include "Observable_impl.h"
#include <pf_imgui/_export.h>
#include <utility>

namespace pf::ui::ig {
/**
 * @brief Interface for focusable elements.
 *
 * Provides a functionality to manage and observe focus changes.
 */
class PF_IMGUI_EXPORT Focusable {
 public:
  /**
   * Add a listener, which is called every time the elements' focus state is changed.
   * @param fnc listener for focus change event, true is passed when the element is focused, false otherwise
   * @return instance of Subscription, which allows to unsubscribe the listener
   * @see Subscription
   */
  Subscription addFocusListener(std::invocable<bool> auto &&fnc) {
    return observableImpl.addListener(std::forward<decltype(fnc)>(fnc));
  }

  /**
   * Check if element is currently focused.
   * @return true if element is focused, false otherwise
   */
  [[deprecated("use hasFocus instead")]] [[nodiscard]] bool isFocused() const;
  /**
   * Check if element is currently focused.
   * @return true if element is focused, false otherwise
   */
  [[nodiscard]] bool hasFocus() const;
  /**
   * Set this object as focused.
   *
   * @attention This method should be overriden in most subclasses to provide proper focus response.
   */
  virtual void setFocus();

  virtual ~Focusable() = default;

 protected:
  /**
   * Try and update focus state. Checks if the new state is different from the current state.
   * If it is a notification of focus state change is sent to listeners.
   * @param focus new focus
   */
  void updateFocused(bool focus);

 private:
  /**
   * Notify all listeners of a focus change.
   * @param focused focus state
   */
  void notifyFocusChanged(bool focused);
  Observable_impl<bool> observableImpl;
  bool focused = false;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_FOCUSABLE_H
