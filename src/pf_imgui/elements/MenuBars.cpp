//
// Created by petr on 11/1/20.
//

#include "MenuBars.h"
#include <imgui.h>

namespace pf::ui::ig {

WindowMenuBar::WindowMenuBar(const std::string &elementName) : Element(elementName) {}

void WindowMenuBar::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (ImGui::BeginMenuBar()) {
    RAII end{ImGui::EndMenuBar};
    renderItems();
  }
}

AppMenuBar::AppMenuBar(const std::string &elementName) : Element(elementName) {}

void AppMenuBar::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (ImGui::BeginMainMenuBar()) {
    RAII end{ImGui::EndMainMenuBar};
    renderItems();
  }
}

}  // namespace pf::ui::ig
