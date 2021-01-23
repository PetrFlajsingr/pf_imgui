//
// Created by petr on 1/23/21.
//

#include "Collapsible.h"

namespace pf::ui::ig {
bool Collapsible::isCollapsed() const { return collapsed; }

void Collapsible::setCollapsed(bool collapse) {
  if (collapse != collapsed) {
    collapsed = collapse;
    collapse_impl(collapsed);
    notifyCollapseChanged(collapsed);
  }
}

void Collapsible::notifyCollapseChanged(bool collapse) { observableImpl.notify(collapse); }
void Collapsible::setCollapsedWithoutDemandingCollapseChange(bool collapse) {
  if (collapse != collapsed) {
    collapsed = collapse;
    notifyCollapseChanged(collapsed);
  }
}
}// namespace pf::ui::ig