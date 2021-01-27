//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_POSITIONABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_POSITIONABLE_H

#include "Observable_impl.h"
#include <imgui.h>

namespace pf::ui::ig {
class Positionable {
 public:
  explicit Positionable(const ImVec2 &position);

  Subscription addPositionListener(std::invocable<ImVec2> auto listener) {
    return observableImpl.template addListener(listener);
  }

  [[nodiscard]] ImVec2 getPosition() const;
  virtual void setPosition(ImVec2 pos);

  virtual ~Positionable() = default;

 protected:
  void updatePosition(ImVec2 pos);

 private:
  ImVec2 position;
  Observable_impl<ImVec2> observableImpl;

  void notifyPositionChanged(ImVec2 pos);
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_POSITIONABLE_H
