//
// Created by petr on 1/23/21.
//

#include "Collapsible.h"

namespace pf::ui::ig {
Collapsible::Collapsible(AllowCollapse allowCollapse, Persistent persistent)
    : Savable(persistent), collapsible(allowCollapse == AllowCollapse::Yes) {}

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

void Collapsible::unserialize_impl(const toml::table &src) {
  if (auto newValIter = src.find("collapsed"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setCollapsed(*newVal); }
  }
}

toml::table Collapsible::serialize_impl() { return toml::table{{{"collapsed", isCollapsed()}}}; }

}// namespace pf::ui::ig