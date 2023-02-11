//
// Created by petr on 11/1/20.
//

#include "MenuBars.h"
#include <imgui.h>

namespace pf::ui::ig {

WindowMenuBar::WindowMenuBar(WindowMenuBar::Config &&config) : WindowMenuBar(config.name) {}

WindowMenuBar::WindowMenuBar(std::string_view elementName) : ElementWithID(elementName) {}

void WindowMenuBar::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::BeginMenuBar()) {
    ScopeExit end{&ImGui::EndMenuBar};
    renderItems();
  }
}

AppMenuBar::AppMenuBar(AppMenuBar::Config &&config) : AppMenuBar(config.name) {}

AppMenuBar::AppMenuBar(std::string_view elementName) : ElementWithID(elementName) {}

void AppMenuBar::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::BeginMainMenuBar()) {
    ScopeExit end{&ImGui::EndMainMenuBar};
    renderItems();
  }
}

}  // namespace pf::ui::ig
