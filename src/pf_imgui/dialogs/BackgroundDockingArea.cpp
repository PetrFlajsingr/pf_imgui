//
// Created by xflajs00 on 14.04.2022.
//

#include "BackgroundDockingArea.h"

namespace pf::ui::ig {

BackgroundDockingArea::BackgroundDockingArea(const std::string &name)
    : Renderable(name), Resizable(Size::Auto()), dockSpace(DockSpace{"background_dock"}) {}

DockSpace &BackgroundDockingArea::getDockSpace() { return dockSpace; }

void BackgroundDockingArea::renderImpl() {
  RAII end{ImGui::End};
  ImGui::SetNextWindowSize(static_cast<ImVec2>(getSize()));
  if (ImGui::Begin("##background_dock_window", nullptr, flags)) {
    const auto viewportSize = ImGui::GetWindowViewport()->Size;
    setSize(Size{Width{viewportSize.x}, Height{viewportSize.y}});
    dockSpace.render();
  }
}

}  // namespace pf::ui::ig