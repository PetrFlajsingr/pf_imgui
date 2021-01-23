//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_FOCUSABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_FOCUSABLE_H

#include "Observable_impl.h"

namespace pf::ui::ig {
class Focusable {
 public:
  Subscription addFocusListener(std::invocable<bool> auto fnc) {
    return observableImpl.template addListener(fnc);
  }

  [[nodiscard]] bool isFocused() const;
  void setFocused(bool newFocused) {
    if (newFocused != focused) {
      focused = newFocused;
      notifyFocusChanged(focused);
      if (focused) { setFocus_impl(); }
    }
  }

  virtual ~Focusable() = default;

 protected:
  virtual void setFocus_impl() = 0;
  void setFocusedWithoutDemandingFocusChange(bool newFocused);

 private:
  void notifyFocusChanged(bool focused);
  Observable_impl<bool> observableImpl;
  bool focused = false;
};
}

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_FOCUSABLE_H
