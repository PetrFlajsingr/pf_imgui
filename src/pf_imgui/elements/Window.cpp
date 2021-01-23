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
  auto flags = hasMenuBar() ? ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar : ImGuiWindowFlags_{};
  ImGui::Begin(title.c_str(), nullptr, flags);
  setHovered(ImGui::IsWindowHovered());

  setCollapsedWithoutDemandingCollapseChange(ImGui::IsWindowCollapsed());
  setFocusedWithoutDemandingFocusChange(ImGui::IsWindowFocused());
  if (!isCollapsed()) {
    if (hasMenuBar()) { menuBar->render(); }
    std::ranges::for_each(getChildren(), [&](auto &child) { child.render(); });
  }
  ImGui::End();
}

const std::string &Window::getTitle() const { return title; }

void Window::setTitle(const std::string &tit) { title = tit; }

WindowMenuBar &Window::getMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<WindowMenuBar>(getName() + "_menu_bar"); }
  return *menuBar;
}

bool Window::hasMenuBar() const { return menuBar != nullptr; }

void Window::removeMenuBar() { menuBar = nullptr; }

const ImVec2 &Window::getPosition() const { return position; }

void Window::setPosition(const ImVec2 &newPosition) {
  position = newPosition;
  ImGui::SetWindowPos(getTitle().c_str(), position);
}

const ImVec2 &Window::getSize() const { return size; }

void Window::setSize(const ImVec2 &newSize) {
  size = newSize;
  ImGui::SetWindowSize(getTitle().c_str(), size);
}

void Window::setFocus_impl() {
  ImGui::SetWindowFocus(getTitle().c_str());
}

void Window::collapse_impl(bool collapse) {
  ImGui::SetWindowCollapsed(getTitle().c_str(), collapse);
}

}// namespace pf::ui::ig