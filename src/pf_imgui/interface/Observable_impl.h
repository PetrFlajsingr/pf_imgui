/**
 * @file Observable_impl.h
 * @brief Implementation of observable.
 * @author Petr Flajšingr
 * @date 23.1.21
 */


#ifndef PF_IMGUI_INTERFACE_OBSERVABLE_IMPL_H
#define PF_IMGUI_INTERFACE_OBSERVABLE_IMPL_H

#include <algorithm>
#include <pf_common/Subscription.h>
#include <pf_common/coroutines/Sequence.h>
#include <pf_imgui/_export.h>
#include <unordered_map>
#include <utility>

namespace pf::ui::ig {
/**
 * @brief Implementation of observable design pattern.
 *
 * Allows multiple observers, each of which has a unique ID.
 * @tparam Args Types of arguments to be used as callback parameters
 */
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

  /**
   * Add a listener and return a Subscription object which can be used to unregister it.
   * @param fnc listener
   * @return Subscription for unregistration purposes
   * @see Subscription
   */
  Subscription addListener(std::invocable<const Args &...> auto fnc) {
    const auto id = generateListenerId();
    listeners[id] = fnc;
    return Subscription([id, this] { listeners.erase(id); });
  }

  /**
   * Notify all listeners with provided parameters.
   * @param args parameters to be passed to listeners
   */
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
#endif//PF_IMGUI_INTERFACE_OBSERVABLE_IMPL_H
