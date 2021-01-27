//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_COLLAPSIBLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_COLLAPSIBLE_H

#include "Observable_impl.h"
#include "Savable.h"
#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Collapsible : public Savable {
 public:
  explicit Collapsible(AllowCollapse allowCollapse, Persistent persistent);

  Subscription addCollapseListener(std::invocable<bool> auto listener) {
    return observableImpl.template addListener(listener);
  }

  [[nodiscard]] bool isCollapsed() const;
  virtual void setCollapsed(bool collapsed);

  [[nodiscard]] bool isCollapsible() const;
  void setCollapsible(bool collapsible);

 protected:
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

 private:
  void notifyCollapseChanged(bool collapse);
  bool collapsed = false;
  bool collapsible;
  Observable_impl<bool> observableImpl;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_COLLAPSIBLE_H
