//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_VALUEOBSERVABLEELEMENT_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_VALUEOBSERVABLEELEMENT_H

#include "Element.h"
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
class PF_IMGUI_EXPORT ValueObservableElement : public virtual Element {
 public:
  using Callback = std::function<void(const T &)>;
  using Id = uint32_t;
  explicit ValueObservableElement(const std::string &elementName, T value = T{}) : Element(elementName), value(value) {}

  ValueObservableElement(ValueObservableElement &&other) noexcept
      : Element(std::move(other)), value(std::move(other.value)), listeners(std::move(other.listeners)),
        idGenerator(std::move(idGenerator)) {}
  ValueObservableElement &operator=(ValueObservableElement &&other) noexcept {
    value = std::move(other.value);
    listeners = std::move(other.listeners);
    idGenerator = std::move(other.idGenerator);
    Element::operator=(std::move(other));
    return *this;
  }

  Subscription addValueListener(std::invocable<const T &> auto fnc, bool callNow = false) {
    if (callNow) { fnc(value); }
    const auto id = generateListenerId();
    listeners[id] = fnc;
    return Subscription([id, this] { listeners.erase(id); });
  }

  Subscription bind(T &toBind) {
    auto bindAddress = &toBind;
    return addValueListener([bindAddress](auto newValue) { *bindAddress = newValue; });
  }

  [[nodiscard]] const T &getValue() const { return value; }

 protected:
  void notifyValueChanged() {
    auto callables = listeners | std::views::values;
    std::ranges::for_each(callables, [&](const auto &callable) { callable(value); });
  }
  void setValue(T val) { value = val; }

  void setValueAndNotifyIfChanged(T val) {
    const auto oldValue = getValue();
    setValue(val);
    if (oldValue != getValue()) { notifyValueChanged(); }
  }

  T *getValueAddress() { return &value; }

 private:
  Id generateListenerId() { return getNext(idGenerator); }

  T value;
  std::unordered_map<Id, Callback> listeners;

  cppcoro::generator<Id> idGenerator = iota<Id>();
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_VALUEOBSERVABLEELEMENT_H
