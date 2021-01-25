//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ANCHORABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ANCHORABLE_H

#include "Element.h"
#include <pf_imgui/enums.h>

namespace pf::ui::ig {
class Anchorable : public virtual Element {
 public:
  explicit Anchorable(Anchor anchor);

  [[nodiscard]] Anchor getAnchor() const;
  void setAnchor(Anchor anchor);

 private:
  Anchor anchor;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ANCHORABLE_H
