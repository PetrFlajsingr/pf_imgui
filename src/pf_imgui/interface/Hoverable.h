/**
 * @file Hoverable.h
 * @brief Interface for elements which can be hovered.
 * @author Petr Flajšingr
 * @date 23.1.20
 */

#ifndef PF_IMGUI_INTERFACE_HOVERABLE_H
#define PF_IMGUI_INTERFACE_HOVERABLE_H

#include <pf_imgui/reactive/Observable_impl.h>
#include <pf_imgui/_export.h>
#include <utility>

namespace pf::ui::ig {
/**
 * @brief Interface for hoverable elements
 *
 * Provides a functionality to manage and observe hover changes.
 */
class PF_IMGUI_EXPORT Hoverable {
 public:
  /**
   * Add a listener, which is called every time the elements' hover state is changed.
   * @param fnc listener for hover change event, true is passed when the element is hovered, false otherwise
   * @return instance of Subscription, which allows to unsubscribe the listener
   * @see Subscription
   */
  Subscription addHoverListener(std::invocable<bool> auto &&fnc) {
    return observableImpl.addListener(std::forward<decltype(fnc)>(fnc));
  }

  /**
   * Check if the element is hovered.
   * @return true if hovered, false otherwise
   */
  [[nodiscard]] bool isHovered() const;

  virtual ~Hoverable() = default;

 protected:
  /**
   * Set new hovered state. If the state is different from the current state the listeners are notified of this change.
   * @param hover new hover state
   */
  void setHovered(bool hover);

 private:
  /**
   * Notify all listeners of new hover state.
   * @param hover new hover state
   */
  void notifyHovered(bool hover);
  Observable_impl<bool> observableImpl;
  bool hovered = false;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_HOVERABLE_H
