/**
 * @file ValueContainer.h
 * @brief Interface for providing inner value access
 * @author Petr Flajšingr
 * @date 4.9.22
 */

#ifndef PF_IMGUI_INTERFACE_VALUECONTAINER_H
#define PF_IMGUI_INTERFACE_VALUECONTAINER_H

#include <concepts>
#include <functional>
#include <pf_common/Subscription.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

template<typename T>
class PF_IMGUI_EXPORT ValueContainer {
 public:
  virtual ~ValueContainer() = default;

  virtual void setValue(const T &newValue) = 0;
  [[nodiscard]] virtual const T &getValue() const = 0;
  Subscription addValueListener(std::invocable<T> auto &&listener) {
    return addValueListenerImpl(std::forward<decltype(listener)>(listener));
  }

 protected:
  virtual Subscription addValueListenerImpl(std::function<void(T)> listener) = 0;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_INTERFACE_VALUECONTAINER_H
