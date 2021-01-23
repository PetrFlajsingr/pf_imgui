//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_HOVERABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_HOVERABLE_H

#include "Observable_impl.h"

namespace pf::ui::ig {
class Hoverable {
 public:
  Subscription addHoverListener(std::invocable<bool> auto fnc) {
    return observableImpl.template addListener(fnc);
  }

 protected:
  void notifyHovered(bool hovered);
 private:
  Observable_impl<bool> observableImpl;
};
}

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_HOVERABLE_H
