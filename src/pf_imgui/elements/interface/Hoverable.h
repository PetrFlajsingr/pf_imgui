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

  [[nodiscard]] bool isHovered() const;

 protected:
  void setHovered(bool hover);
 private:
  void notifyHovered(bool hover);
  Observable_impl<bool> observableImpl;
  bool hovered = false;
};
}

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_HOVERABLE_H
