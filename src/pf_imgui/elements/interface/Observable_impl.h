//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_OBSERVABLE_IMPL_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_OBSERVABLE_IMPL_H

#include <algorithm>
#include <pf_common/Subscription.h>
#include <pf_common/coroutines/Sequence.h>
#include <pf_imgui/_export.h>
#include <unordered_map>
#include <utility>

namespace pf::ui::ig {
template<typename... Args>
class PF_IMGUI_EXPORT Observable_impl {
 public:
  Observable_impl() = default;
  Observable_impl(Observable_impl &&other) noexcept
      : listeners(std::move(other.listeners)), idGenerator(std::move(other.idGenerator)) {}
  Observable_impl &operator=(Observable_impl &&rhs) noexcept {
    listeners = std::move(rhs.listeners);
    idGenerator = std::move(rhs.idGenerator);
    return *this;
  }

  Subscription addListener(std::invocable<const Args &...> auto fnc) {
    const auto id = generateListenerId();
    listeners[id] = fnc;
    return Subscription([id, this] { listeners.erase(id); });
  }

  void notify(const Args &...args) {
    auto callables = listeners | std::views::values;
    std::ranges::for_each(callables, [&](const auto &callable) { callable(args...); });
  }

 private:
  using ListenerId = uint32_t;
  using Callback = std::function<void(const Args &...)>;
  ListenerId generateListenerId() { return getNext(idGenerator); }

  std::unordered_map<ListenerId, Callback> listeners{};
  cppcoro::generator<ListenerId> idGenerator = iota<ListenerId>();
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_OBSERVABLE_IMPL_H
