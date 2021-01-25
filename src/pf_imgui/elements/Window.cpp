//
// Created by petr on 10/31/20.
//

#include "Window.h"
#include <algorithm>
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

Window::Window(std::string elementName, std::string title)
    : Resizable(ImVec2(0, 0)), name(std::move(elementName)), title(std::move(title)) {}

void Window::renderImpl() {
  auto flags = hasMenuBar() ? ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar : ImGuiWindowFlags_{};
  if (ImGui::Begin(title.c_str(), nullptr, flags)) {
    if (getEnabled() == Enabled::No) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    {
      auto raiiEnabled = pf::RAII([this] {
        if (getEnabled() == Enabled::No) {
          ImGui::PopItemFlag();
          ImGui::PopStyleVar();
        }
      });
      setHovered(ImGui::IsWindowHovered());

      setCollapsedWithoutDemandingCollapseChange(ImGui::IsWindowCollapsed());
      setFocusedWithoutDemandingFocusChange(ImGui::IsWindowFocused());
      if (!isCollapsed()) {
        if (hasMenuBar()) { menuBar->render(); }
        std::ranges::for_each(getChildren(), [&](auto &child) { child.render(); });
      }
    }
  }
  ImGui::End();
}

const std::string &Window::getTitle() const { return title; }

void Window::setTitle(const std::string &tit) { title = tit; }

WindowMenuBar &Window::getMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<WindowMenuBar>(std::string("getName()") + "_menu_bar"); }
  return *menuBar;
}

bool Window::hasMenuBar() const { return menuBar != nullptr; }

void Window::removeMenuBar() { menuBar = nullptr; }

const ImVec2 &Window::getPosition() const { return position; }

void Window::setPosition(const ImVec2 &newPosition) {
  position = newPosition;
  ImGui::SetWindowPos(getTitle().c_str(), position);
}

void Window::setSize(const ImVec2 &newSize) {
  Resizable::setSize(newSize);
  ImGui::SetWindowSize(getTitle().c_str(), getSize());
}

void Window::setFocus_impl() { ImGui::SetWindowFocus(getTitle().c_str()); }

void Window::collapse_impl(bool collapse) { ImGui::SetWindowCollapsed(getTitle().c_str(), collapse); }

void Window::render() {
  if (getVisibility() == Visibility::Visible) { renderImpl(); }
}
const std::string &Window::getName() const { return name; }

}// namespace pf::ui::ig