//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_FOCUSABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_FOCUSABLE_H

#include "Observable_impl.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Focusable {
 public:
  Subscription addFocusListener(std::invocable<bool> auto fnc) { return observableImpl.template addListener(fnc); }

  [[nodiscard]] bool isFocused() const;
  virtual void setFocus();

  virtual ~Focusable() = default;

 protected:
  void updateFocused(bool focus);

 private:
  void notifyFocusChanged(bool focused);
  Observable_impl<bool> observableImpl;
  bool focused = false;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_FOCUSABLE_H
