//
// Created by petr on 11/1/20.
//

#include "MenuBars.h"
#include <algorithm>
#include <imgui.h>
#include <imgui_internal.h>
#include <utility>

namespace pf::ui::ig {

WindowMenuBar::WindowMenuBar(const std::string &elementName) : Element(elementName) {}
void WindowMenuBar::renderImpl() {
  if (ImGui::BeginMenuBar()) {
    renderItems();
    ImGui::EndMenuBar();
  }
}

AppMenuBar::AppMenuBar(const std::string &elementName) : Element(elementName) {}

void AppMenuBar::renderImpl() {
  if (ImGui::BeginMainMenuBar()) {
    renderItems();
    ImGui::EndMainMenuBar();
  }
}

}// namespace pf::ui::ig
