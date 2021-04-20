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

Window::Window(std::string name, std::string label, AllowCollapse allowCollapse, Persistent persistent)
    : Renderable(std::move(name)), Collapsible(allowCollapse, persistent), Resizable(ImVec2(0, 0)),
      Positionable(ImVec2{}), Labellable(std::move(label)) {}

Window::Window(std::string name, std::string label, Persistent persistent)
    : Window(std::move(name), std::move(label), AllowCollapse::No, persistent) {}

void Window::renderImpl() {
  auto flags = createWindowFlags();
  if (ImGui::Begin(getLabel().c_str(), nullptr, flags)) {
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
      Collapsible::setCollapsed(ImGui::IsWindowCollapsed());
      updateFocused(ImGui::IsWindowFocused());
      updatePosition(ImGui::GetWindowPos());
      if (!isCollapsed()) {
        if (hasMenuBar()) { menuBar->render(); }
        std::ranges::for_each(getChildren(), [&](auto &child) { child.render(); });
      }
    }
  }
  ImGui::End();
}

WindowMenuBar &Window::getMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<WindowMenuBar>(getName() + "_menu_bar"); }
  return *menuBar;
}

bool Window::hasMenuBar() const { return menuBar != nullptr; }

void Window::removeMenuBar() { menuBar = nullptr; }
void Window::setSize(const ImVec2 &newSize) {
  Resizable::setSize(newSize);
  ImGui::SetWindowSize(getLabel().c_str(), getSize());
}

void Window::render() {
  if (getVisibility() == Visibility::Visible) { renderImpl(); }
}

void Window::setCollapsed(bool collapsed) {
  ImGui::SetWindowCollapsed(getLabel().c_str(), collapsed);
  Collapsible::setCollapsed(collapsed);
}
void Window::setFocus() {
  ImGui::SetWindowFocus(getLabel().c_str());
  Focusable::setFocus();
}
void Window::setPosition(ImVec2 pos) {
  ImGui::SetWindowPos(getLabel().c_str(), pos);
  Positionable::setPosition(pos);
}
ImGuiWindowFlags Window::createWindowFlags() {
  ImGuiWindowFlags result = hasMenuBar() ? ImGuiWindowFlags_MenuBar : ImGuiWindowFlags{};
  if (!isCollapsible()) {
    result |= ImGuiWindowFlags_NoCollapse;
  }
  if (getLabel().empty()) {
    result |= ImGuiWindowFlags_NoTitleBar;
  }
  return result;
}

}// namespace pf::ui::ig