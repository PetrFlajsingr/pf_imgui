//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ANCHORABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ANCHORABLE_H

#include <pf_imgui/enums.h>

namespace pf::ui::ig {
class Anchorable {
 public:
  explicit Anchorable(Anchor anchor);

  [[nodiscard]] Anchor getAnchor() const;
  void setAnchor(Anchor anchor);

  ~Anchorable() = default;

 private:
  Anchor anchor;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ANCHORABLE_H
