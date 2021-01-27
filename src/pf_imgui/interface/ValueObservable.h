//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_VALUEOBSERVABLEELEMENT_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_VALUEOBSERVABLEELEMENT_H

#include "Observable_impl.h"
#include <algorithm>
#include <functional>
#include <pf_common/Subscription.h>
#include <pf_common/coroutines/Sequence.h>
#include <pf_imgui/_export.h>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>

namespace pf::ui::ig {
template<typename T>
class PF_IMGUI_EXPORT ValueObservable {
 public:
  explicit ValueObservable(T value = T{}) : value(value) {}

  ValueObservable(ValueObservable &&other) noexcept : value(std::move(other.value)) {}

  ValueObservable &operator=(ValueObservable &&other) noexcept {
    value = std::move(other.value);
    observableImpl = std::move(other.observableImpl);
    return *this;
  }

  Subscription addValueListener(std::invocable<const T &> auto fnc, bool callNow = false) {
    if (callNow) { fnc(value); }
    return observableImpl.template addListener(fnc);
  }

  Subscription bind(T &toBind) {
    auto bindAddress = &toBind;
    return addValueListener([bindAddress](auto newValue) { *bindAddress = newValue; });
  }

  [[nodiscard]] const T &getValue() const { return value; }

 protected:
  void notifyValueChanged() { observableImpl.notify(value); }
  void setValue(T val) { value = val; }

  void setValueAndNotifyIfChanged(T val) {
    const auto oldValue = getValue();
    setValue(val);
    if (oldValue != getValue()) { notifyValueChanged(); }
  }

  T *getValueAddress() { return &value; }

 private:
  T value;
  Observable_impl<T> observableImpl;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_VALUEOBSERVABLEELEMENT_H
