//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_COLLAPSIBLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_COLLAPSIBLE_H

#include "Observable_impl.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Collapsible {
 public:
  Subscription addCollapseListener(std::invocable<bool> auto listener) {
    return observableImpl.template addListener(listener);
  }

  [[nodiscard]] bool isCollapsed() const;
  virtual void setCollapsed(bool collapsed);

  virtual ~Collapsible() = default;

 private:
  void notifyCollapseChanged(bool collapse);
  bool collapsed = false;
  Observable_impl<bool> observableImpl;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_COLLAPSIBLE_H
