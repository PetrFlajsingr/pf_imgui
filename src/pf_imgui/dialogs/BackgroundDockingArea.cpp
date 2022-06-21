//
// Created by xflajs00 on 14.04.2022.
//

#include "BackgroundDockingArea.h"
#include <imgui_internal.h>

namespace pf::ui::ig {

BackgroundDockingArea::BackgroundDockingArea(const std::string &name)
    : Renderable(name), size(Size::Auto()), dockSpace(DockSpace{"background_dock"}) {}

DockSpace &BackgroundDockingArea::getDockSpace() { return dockSpace; }

Size BackgroundDockingArea::getSize() const { return size; }

void BackgroundDockingArea::renderImpl() {
  RAII end{ImGui::End};
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size - leftTopMargin - bottomRightMargin);
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos + leftTopMargin);
  if (ImGui::Begin("##background_dock_window", nullptr, flags)) {
    const auto viewportSize = ImGui::GetMainViewport()->Size;
    size = Size{Width{viewportSize.x}, Height{viewportSize.y}};
    dockSpace.render();
  }
}

}  // namespace pf::ui::ig