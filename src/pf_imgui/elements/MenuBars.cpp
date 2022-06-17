//
// Created by petr on 11/1/20.
//

#include "MenuBars.h"
#include <imgui.h>

namespace pf::ui::ig {

WindowMenuBar::WindowMenuBar(WindowMenuBar::Config &&config) : ElementWithID(std::string{config.name.value}) {}

WindowMenuBar::WindowMenuBar(const std::string &elementName) : ElementWithID(elementName) {}

void WindowMenuBar::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  if (ImGui::BeginMenuBar()) {
    RAII end{ImGui::EndMenuBar};
    renderItems();
  }
}

AppMenuBar::AppMenuBar(AppMenuBar::Config &&config) : ElementWithID(std::string{config.name.value}) {}

AppMenuBar::AppMenuBar(const std::string &elementName) : ElementWithID(elementName) {}

void AppMenuBar::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  if (ImGui::BeginMainMenuBar()) {
    RAII end{ImGui::EndMainMenuBar};
    renderItems();
  }
}

}  // namespace pf::ui::ig
