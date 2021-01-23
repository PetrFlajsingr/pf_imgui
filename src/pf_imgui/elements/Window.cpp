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
  const auto wasHovered = hovered;
  hovered = ImGui::IsWindowHovered();
  if (hovered != wasHovered) {
    notifyHovered(hovered);
  }
  const auto wasCollapsed = collapsed;
  collapsed = ImGui::IsWindowCollapsed();
  if (collapsed != wasCollapsed) {
    notifyCollapseChanged();
  }
  const auto wasFocused = focused;
  focused = ImGui::IsWindowFocused();
  if (focused != wasFocused) {
    notifyFocusChanged(focused);
  }
  if (!collapsed) {
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

bool Window::isCollapsed() const { return collapsed; }

void Window::setCollapsed(bool newCollapsed) {
  collapsed = newCollapsed;
  ImGui::SetWindowCollapsed(getTitle().c_str(), collapsed);
  notifyCollapseChanged();
}

bool Window::isHovered() const { return hovered; }

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

bool Window::isFocused() const { return focused; }

void Window::setFocused(bool newFocused) {
  focused = newFocused;
  ImGui::SetWindowFocus(getTitle().c_str());
  notifyFocusChanged(focused);
}
void Window::notifyCollapseChanged() {
  observableImplCollapse.notify(focused);
}

}// namespace pf::ui::ig