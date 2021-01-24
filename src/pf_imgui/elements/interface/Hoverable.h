//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_HOVERABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_HOVERABLE_H

#include "Observable_impl.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Hoverable {
 public:
  Subscription addHoverListener(std::invocable<bool> auto fnc) { return observableImpl.template addListener(fnc); }

  [[nodiscard]] bool isHovered() const;

  virtual ~Hoverable() = default;

 protected:
  void setHovered(bool hover);

 private:
  void notifyHovered(bool hover);
  Observable_impl<bool> observableImpl;
  bool hovered = false;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_HOVERABLE_H
