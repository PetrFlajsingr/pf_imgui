//
// Created by xflajs00 on 15.11.2021.
//

#include "StatusBar.h"
#include <fmt/format.h>
#include <string>

namespace pf::ui::ig {

AppStatusBar::AppStatusBar(AppStatusBar::Config &&config) : ElementWithID(std::string{config.name.value}) {}

AppStatusBar::AppStatusBar(const std::string &name) : ElementWithID(name) {}

void AppStatusBar::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  ImGui::GetFrameHeight();
  auto *viewport = reinterpret_cast<ImGuiViewportP *>(ImGui::GetMainViewport());
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
  height = ImGui::GetFrameHeight();
  if (ImGui::BeginViewportSideBar(fmt::format("##{}", getName()).c_str(), viewport, ImGuiDir_Down, height,
                                  window_flags)) {
    RAII end{ImGui::End};
    if (ImGui::BeginMenuBar()) {
      RAII endMenu{ImGui::EndMenuBar};
      std::ranges::for_each(getChildren(), &Renderable::render);
    }
  }
}

float AppStatusBar::getHeight() const { return height; }

}  // namespace pf::ui::ig
