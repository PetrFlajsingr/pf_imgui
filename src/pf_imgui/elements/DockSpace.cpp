//
// Created by petr on 6/27/21.
//

#include "DockSpace.h"
#include <pf_common/RAII.h>

namespace pf::ui::ig {

DockSpace::DockSpace(Config &&config)
    : Element(std::string{config.name}), Resizable(config.size), flags(*config.flags) {}

DockSpace::DockSpace(const std::string &name, Size s, const Flags<DockType> &dockFlags)
    : Element(name), Resizable(s), flags(*dockFlags) {}

void DockSpace::render() {
  if (getVisibility() == Visibility::Visible) {
    auto colorSet = setColorStack();
    if (getEnabled() == Enabled::No) {
      ImGui::BeginDisabled();
      RAII raiiEnabled{ImGui::EndDisabled};
      renderImpl();
    } else {
      renderImpl();
    }
  }
}

void DockSpace::renderImpl() {
  ImGui::DockSpace(getId(), static_cast<ImVec2>(getSize()), flags);
}

}  // namespace pf::ui::ig
