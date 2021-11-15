//
// Created by xflajs00 on 15.11.2021.
//

#include "StatusBar.h"

namespace pf::ui::ig {

AppStatusBar::AppStatusBar(const std::string &name) : Element(name) {}

void AppStatusBar::renderImpl() {
  ImGui::GetFrameHeight();
  auto *viewport = (ImGuiViewportP *) (void *) ImGui::GetMainViewport();
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
  const auto height = ImGui::GetFrameHeight();
  if (ImGui::BeginViewportSideBar(fmt::format("##{}", getName()).c_str(), viewport, ImGuiDir_Down, height,
                                  window_flags)) {
    if (ImGui::BeginMenuBar()) {
      std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
      ImGui::EndMenuBar();
    }
    ImGui::End();
  }
}

}// namespace pf::ui::ig