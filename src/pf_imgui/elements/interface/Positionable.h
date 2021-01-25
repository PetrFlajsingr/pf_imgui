//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_POSITIONABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_POSITIONABLE_H

#include "Element.h"
#include <imgui.h>

namespace pf::ui::ig {
class Positionable : public virtual Element {
 public:
  explicit Positionable(const ImVec2 &position);
  [[nodiscard]] ImVec2 getPosition() const;
  void setPosition(ImVec2 pos);

 private:
  ImVec2 position;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_POSITIONABLE_H
