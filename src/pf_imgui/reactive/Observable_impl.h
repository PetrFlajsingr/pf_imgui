/**
 * @file Observable_impl.h
 * @brief Implementation of observable.
 * @author Petr Flajšingr
 * @date 23.1.21
 */

#ifndef PF_IMGUI_REACTIVE_OBSERVABLE_IMPL_H
#define PF_IMGUI_REACTIVE_OBSERVABLE_IMPL_H

#include <algorithm>
#include <memory>
#include <pf_common/Subscription.h>
#include <pf_imgui/_export.h>
#include <ranges>
#include <utility>
#include <vector>

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
  Observable_impl(const Observable_impl &) = delete;
  Observable_impl &operator=(const Observable_impl &) = delete;

  Observable_impl(Observable_impl &&other) noexcept;
  Observable_impl &operator=(Observable_impl &&rhs) noexcept;

  virtual ~Observable_impl();

  /**
   * Add a listener and return a Subscription object which can be used to unregister it.
   * @param fnc listener
   * @return Subscription for unregistration purposes
   * @see Subscription
   */
  Subscription addListener(std::invocable<const Args &...> auto &&fnc);

  /**
   * Notify all listeners with provided parameters.
   * @param args parameters to be passed to listeners
   */
  void notify(const Args &...args);

 private:
  using ListenerId = uint32_t;
  using Callback = std::function<void(const Args &...)>;
  using ListenerRecord = std::pair<ListenerId, Callback>;
  static ListenerId generateListenerId() { return idCounter++; }

  std::vector<ListenerRecord> listeners{};
  inline static ListenerId idCounter{};
  std::shared_ptr<bool> exists = std::make_shared<bool>(true);
};

template<typename... Args>
Observable_impl<Args...>::Observable_impl(Observable_impl &&other) noexcept : listeners(std::move(other.listeners)) {}

template<typename... Args>
Observable_impl<Args...> &Observable_impl<Args...>::operator=(Observable_impl &&rhs) noexcept {
  listeners = std::move(rhs.listeners);
  return *this;
}

template<typename... Args>
Observable_impl<Args...>::~Observable_impl() {
  *exists = false;
}

template<typename... Args>
Subscription Observable_impl<Args...>::addListener(std::invocable<const Args &...> auto &&fnc) {
  const auto id = generateListenerId();
  listeners.emplace_back(id, std::forward<decltype(fnc)>(fnc));
  return Subscription([id, this, observableExists = exists] {
    if (!*observableExists) { return; }
    auto [removeB, removeE] = std::ranges::remove(listeners, id, &ListenerRecord::first);
    listeners.erase(removeB, removeE);
  });
}

template<typename... Args>
void Observable_impl<Args...>::notify(const Args &...args) {
  auto callables = listeners | std::views::values;
  std::ranges::for_each(callables, [&](const auto &callable) { callable(args...); });
}

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_REACTIVE_OBSERVABLE_IMPL_H
