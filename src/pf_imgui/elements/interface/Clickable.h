//
// Created by petr on 11/14/20.
//

#ifndef PF_IMGUI_ELEMENTS_INTERFACE_CLICKABLE_H
#define PF_IMGUI_ELEMENTS_INTERFACE_CLICKABLE_H

#include <functional>
#include <pf_common/Subscription.h>
#include <pf_common/coroutines/Sequence.h>
#include <unordered_map>

namespace pf::ui::ig {

class Clickable {
 public:
  using Callback = std::function<void()>;
  using Id = uint32_t;

  Subscription addClickListener(std::invocable auto fnc) {
    const auto id = generateListenerId();
    listeners[id] = fnc;
    return Subscription([id, this] { listeners.erase(id); });
  }

 protected:
  void notifyOnClick();

 private:
  Id generateListenerId();

  std::unordered_map<Id, Callback> listeners;
  cppcoro::generator<Id> idGenerator = iota<Id>();
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_INTERFACE_CLICKABLE_H
