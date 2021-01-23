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

 protected:
  void notifyFocusChanged(bool focused);
 private:
  Observable_impl<bool> observableImpl;
};
}

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_FOCUSABLE_H
