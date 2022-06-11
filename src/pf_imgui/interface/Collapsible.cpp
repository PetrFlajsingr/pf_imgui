//
// Created by petr on 1/23/21.
//

#include "Collapsible.h"

namespace pf::ui::ig {
Collapsible::Collapsible(AllowCollapse allowCollapse, Persistent persistent)
    : Collapsible(allowCollapse, false, persistent) {}

Collapsible::Collapsible(AllowCollapse allowCollapse, bool initCollapsed, Persistent persistent)
    : Savable(persistent), collapsed(initCollapsed), collapsible(allowCollapse == AllowCollapse::Yes) {}

bool Collapsible::isCollapsed() const { return collapsed; }

void Collapsible::setCollapsed(bool collapse) {
  if (collapse != collapsed) {
    collapsed = collapse;
    notifyCollapseChanged(collapsed);
  }
}
void Collapsible::notifyCollapseChanged(bool collapse) { observableImpl.notify(collapse); }

bool Collapsible::isCollapsible() const { return collapsible; }

void Collapsible::setCollapsible(bool newCollapsible) { collapsible = newCollapsible; }

toml::table Collapsible::toToml() const { return toml::table{{"collapsed", isCollapsed()}}; }

void Collapsible::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("collapsed"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setCollapsed(*newVal); }
  }
}

}  // namespace pf::ui::ig
