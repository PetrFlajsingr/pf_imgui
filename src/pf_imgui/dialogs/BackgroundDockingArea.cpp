//
// Created by xflajs00 on 14.04.2022.
//

#include "BackgroundDockingArea.h"
#include <imcmd_command_palette.h>
#include <imgui_internal.h>

namespace pf::ui::ig {

BackgroundDockingArea::BackgroundDockingArea(std::string_view elementName) : Renderable(elementName) {}

void BackgroundDockingArea::renderImpl() {
  [[maybe_unused]] const auto colorScoped = color.applyScoped();
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

}  // namespace pf::ui::ig