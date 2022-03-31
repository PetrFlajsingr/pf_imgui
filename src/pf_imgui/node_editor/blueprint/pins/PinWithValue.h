//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_PINWITHVALUE_H
#define IMGUI_EXPERIMENTS_PINWITHVALUE_H

#include "ValuePin.h"
#include <imgui_internal.h>

namespace pf::ui::ig::bp {

template<typename T>
  requires(std::equality_comparable<T> && std::is_assignable_v<T &, T> && std::copy_constructible<T>)
class PinWithValue : public ValuePin<T> {
 public:
  using ValueType = typename ValuePin<T>::ValueType;
  using ValuePin<T>::ValuePin;

  [[nodiscard]] virtual T getValue() const = 0;
  virtual void setValue(const T &value) = 0;
};

}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_PINWITHVALUE_H
