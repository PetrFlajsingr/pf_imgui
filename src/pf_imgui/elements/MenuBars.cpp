//
// Created by petr on 11/1/20.
//

#include "MenuBars.h"
#include <imgui.h>

namespace pf::ui::ig {

WindowMenuBar::WindowMenuBar(WindowMenuBar::Config &&config) : ElementWithID(std::string{config.name.value}) {}

WindowMenuBar::WindowMenuBar(const std::string &elementName) : ElementWithID(elementName) {}

void WindowMenuBar::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::BeginMenuBar()) {
    RAII end{ImGui::EndMenuBar};
    renderItems();
  }
}

AppMenuBar::AppMenuBar(AppMenuBar::Config &&config) : ElementWithID(std::string{config.name.value}) {}

AppMenuBar::AppMenuBar(const std::string &elementName) : ElementWithID(elementName) {}

void AppMenuBar::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::BeginMainMenuBar()) {
    RAII end{ImGui::EndMainMenuBar};
    renderItems();
  }
}

}  // namespace pf::ui::ig
