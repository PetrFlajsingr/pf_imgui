/**
 * @file Event.h
 * @brief Events.
 * @author Petr Flajšingr
 * @date 2.9.22
 */

#ifndef PF_IMGUI_REACTIVE_EVENT_H
#define PF_IMGUI_REACTIVE_EVENT_H

#include <pf_imgui/reactive/Observable_impl.h>

namespace pf::ui::ig {

template<typename Owner, typename... Args>
  requires(std::is_class_v<Owner>)
class PF_IMGUI_EXPORT ClassEvent {
  friend Owner;

 public:
  Subscription addListener(std::invocable<const Args &...> auto &&listener) {
    return observableImpl.addListener(std::forward<decltype(listener)>(listener));
  }

 private:
  void notify(Args &&...args) { observableImpl.notify(std::forward<Args>(args)...); }

  Observable_impl<Args...> observableImpl;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_REACTIVE_EVENT_H
