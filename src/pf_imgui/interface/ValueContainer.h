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
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Tags.h>

namespace pf::ui::ig {

namespace details {
template<typename T>
class PF_IMGUI_EXPORT ValueContainerSetValue {
 public:
  virtual void setValue(const T &newValue) = 0;
};
class PF_IMGUI_EXPORT ValueContainerEmptyBase {};
}  // namespace details

template<typename T, OneOf<ReadOnlyTag, ReadWriteTag> Tag = ReadWriteTag>
class PF_IMGUI_EXPORT ValueContainer
    : public std::conditional_t<std::same_as<Tag, ReadWriteTag>, details::ValueContainerSetValue<T>, details::ValueContainerEmptyBase> {
 public:
  using ValueType = T;
  constexpr static bool ReadOnly = std::same_as<Tag, ReadOnlyTag>;

  virtual ~ValueContainer() = default;

  [[nodiscard]] virtual const T &getValue() const = 0;
  Subscription addValueListener(std::invocable<T> auto &&listener) {
    return addValueListenerImpl(std::forward<decltype(listener)>(listener));
  }

 protected:
  virtual Subscription addValueListenerImpl(std::function<void(const T &)> listener) = 0;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_INTERFACE_VALUECONTAINER_H
