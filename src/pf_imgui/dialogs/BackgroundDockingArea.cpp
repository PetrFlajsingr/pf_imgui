//
// Created by xflajs00 on 14.04.2022.
//

#include "BackgroundDockingArea.h"

namespace pf::ui::ig {

BackgroundDockingArea::BackgroundDockingArea(const std::string &name)
    : Renderable(name), size(Size::Auto()), dockSpace(DockSpace{"background_dock"}) {}

DockSpace &BackgroundDockingArea::getDockSpace() { return dockSpace; }

Size BackgroundDockingArea::getSize() const { return size; }

void BackgroundDockingArea::renderImpl() {
  RAII end{ImGui::End};
  ImGui::SetNextWindowSize(static_cast<ImVec2>(getSize()));
  ImGui::SetNextWindowPos(ImVec2{0, 0});
  if (ImGui::Begin("##background_dock_window", nullptr, flags)) {
    const auto viewportSize = ImGui::GetWindowViewport()->Size;
    size = Size{Width{viewportSize.x}, Height{viewportSize.y}};
    dockSpace.render();
  }
}

}  // namespace pf::ui::ig