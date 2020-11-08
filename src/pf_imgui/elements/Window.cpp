//
// Created by petr on 10/31/20.
//

#include "Window.h"
#include <algorithm>
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

Window::Window(const std::string &elementName, std::string title)
    : Element(elementName), Container(elementName), title(std::move(title)) {}

void Window::renderImpl() {
  ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
  if (hasMenuBar()) { menuBar->render(); }
  std::ranges::for_each(getChildren(), [&](auto &child) { child.get().render(); });
  ImGui::End();
}

const std::string &Window::getTitle() const { return title; }

void Window::setTitle(const std::string &tit) { title = tit; }

WindowMenuBar &Window::getMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<WindowMenuBar>(getName() + "_menu_bar"); }
  return *menuBar;
}

bool Window::hasMenuBar() const { return menuBar == nullptr; }
}// namespace pf::ui::ig