//
// Created by petr on 6/27/21.
//

#include "DockSpace.h"
#include <pf_common/RAII.h>

namespace pf::ui::ig {

DockSpace::DockSpace(Config &&config)
    : ElementWithID(std::string{config.name.value}), size(config.size), flags(*config.flags) {}

DockSpace::DockSpace(const std::string &elementName, Size s, const Flags<DockType> &dockFlags)
    : ElementWithID(elementName), size(s), flags(*dockFlags) {}

bool DockSpace::isInitialised() const { return !firstFrame; }

void DockSpace::render() {
  if (firstFrame) {
    firstFrame = false;
    setId(ImGui::GetID(getName().c_str()));
  }
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  Renderable::render();
}

void DockSpace::renderImpl() { setId(ImGui::DockSpace(getId(), static_cast<ImVec2>(*size), flags)); }

}  // namespace pf::ui::ig
