/**
 * @file ValueObservable.h
 * @brief Interface for providing observable support to elements.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_INTERFACE_VALUEOBSERVABLE_H
#define PF_IMGUI_INTERFACE_VALUEOBSERVABLE_H

#include "Observable_impl.h"
#include <algorithm>
#include <concepts>
#include <functional>
#include <pf_common/Subscription.h>
#include <pf_imgui/_export.h>
#include <ranges>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace pf::ui::ig {

namespace details {
template<std::equality_comparable T>
struct DefaultComparator {
  [[nodiscard]] bool operator()(const T &lhs, const T &rhs) const { return lhs == rhs; }
};
}  // namespace details

template<typename T, typename ValueType>
concept ValueObservableComparator = std::invocable<T, ValueType, ValueType>
    && std::same_as<std::invoke_result_t<T, ValueType, ValueType>, bool> && std::is_default_constructible_v<T>;
/**
 * @brief Interface for elements with observable values.
 *
 * Provides storage and functionality to notify listeners of value changes.
 *
 * @tparam type of stored data
 *
 * @todo: drop the copy constructible requirement
 */
template<typename T, ValueObservableComparator<T> Comparator = details::DefaultComparator<T>>
  requires(std::is_assignable_v<T &, T> && std::copy_constructible<T>)
class PF_IMGUI_EXPORT ValueObservable {
  [[no_unique_address]] Comparator comparator{};

 public:
  using ValueType = T;
  /**
  * Construct ValueObservable with starting value.
  * @param value starting value
  */
  explicit ValueObservable(T value = T{}) : value(value) {}

  ValueObservable(ValueObservable &&other) noexcept
    requires(std::is_move_constructible_v<T>)
  : value(std::move(other.value)), observableImpl(std::move(other.observableImpl)) {}

  ValueObservable &operator=(ValueObservable &&other) noexcept
    requires(std::is_move_assignable_v<T>)
  {
    value = std::move(other.value);
    observableImpl = std::move(other.observableImpl);
    return *this;
  }

  /**
   * Add a listener, which is called every time the stored value is changed.
   * @param fnc listener for value change event, inner value is passed as a parameter when it is changed
   * @param callNow if set to true the listener is called right away
   * @return instance of Subscription, which allows to unsubscribe the listener
   * @see Subscription
   */
  Subscription addValueListener(std::invocable<const T &> auto &&fnc, bool callNow = false) {
    if (callNow) { fnc(value); }
    return observableImpl.addListener(std::forward<decltype(fnc)>(fnc));
  }

  /**
   * Bind provided references value to the inner value of ValueObservable.
   * @attention Don't forget to unsubscribe from this before your value is destroyed.
   * @param toBind value to be bound
   * @return instance of Subscription, which allows to unsubscribe the binding
   * @see Subscription
   */
  Subscription bind(T &toBind) {
    toBind = getValue();
    auto bindAddress = &toBind;
    return addValueListener([bindAddress](auto newValue) { *bindAddress = newValue; });
  }

  /**
   * Set value and notify listeners if the value is different.
   * @param newValue value to set
   */
  virtual void setValue(const T &newValue) { setValueAndNotifyIfChanged(newValue); }

  /**
   * Get inner value.
   * @return value
   */
  [[nodiscard]] const T &getValue() const { return value; }

 protected:
  /**
   * Notify all listeners of value change.
   */
  void notifyValueChanged() { observableImpl.notify(value); }
  /**
   * Set inner value.
   * @param val new value
   */
  void setValueInner(T val) { value = val; }

  /**
   * Set new value and check, if it differs from the previous one. If it does notify all listeners.
   * @param val
   */
  void setValueAndNotifyIfChanged(T val) {
    const auto oldValue = getValue();
    setValueInner(val);
    if (!comparator(oldValue, getValue())) { notifyValueChanged(); }
  }

  /**
   * Get pointer to the inner value. Useful for ImGui functions which use pointer for element value representation (Checkbox for example).
   * @return pointer to inner value
   */
  T *getValueAddress() { return &value; }

 private:
  T value;
  Observable_impl<T> observableImpl;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_INTERFACE_VALUEOBSERVABLE_H
