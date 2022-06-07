//
// Created by petr on 6/27/21.
//

#include "DockSpace.h"
#include <pf_common/RAII.h>

namespace pf::ui::ig {

DockSpace::DockSpace(Config &&config)
    : Element(std::string{config.name.value}), Resizable(config.size), flags(*config.flags) {}

DockSpace::DockSpace(const std::string &name, Size s, const Flags<DockType> &dockFlags)
    : Element(name), Resizable(s), flags(*dockFlags) {}

bool DockSpace::isInitialised() const { return !firstFrame; }

void DockSpace::render() {
  if (firstFrame) {
    firstFrame = false;
    setId(ImGui::GetID(getName().c_str()));
  }
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  Renderable::render();
}

void DockSpace::renderImpl() { setId(ImGui::DockSpace(getId(), static_cast<ImVec2>(getSize()), flags)); }

}  // namespace pf::ui::ig
