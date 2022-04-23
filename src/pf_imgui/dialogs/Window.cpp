//
// Created by petr on 10/31/20.
//

#include "Window.h"
#include <algorithm>
#include <imgui.h>
#include <limits>
#include <utility>
#include <vector>

namespace pf::ui::ig {

Window::Window(std::string name, std::string label, AllowCollapse allowCollapse, Persistent persistent)
    : Renderable(std::move(name)), Collapsible(allowCollapse, persistent), Resizable(Size::Auto()),
      Positionable(Position{-1, -1}), Labellable(std::move(label)) {
  refreshIdLabel();
  addLabelListener([this](auto) { refreshIdLabel(); });
}

Window::Window(std::string name, std::string label, Persistent persistent)
    : Window(std::move(name), std::move(label), AllowCollapse::No, persistent) {}

void Window::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  auto flags = createWindowFlags();
  auto isNotClosed = true;

  if (sizeDirty) {
    sizeDirty = false;
    ImGui::SetNextWindowSize(static_cast<ImVec2>(getSize()));
  }
  if (positionDirty) {
    positionDirty = false;
    ImGui::SetNextWindowPos(static_cast<ImVec2>(getPosition()));
  }

  RAII endPopup{ImGui::End};
  if (ImGui::Begin(idLabel.c_str(), (isCloseable() ? &isNotClosed : nullptr), flags)) {
    isWindowDocked = ImGui::IsWindowDocked();
    if (firstPass) {
      firstPass = false;
      if (getSize() != Size::Auto()) { setSize(getSize()); }
      if (getPosition().x != -1 && getPosition().y != -1) { setPosition(getPosition()); }
    }
    if (getEnabled() == Enabled::No) { ImGui::BeginDisabled(); }
    {
      auto raiiEnabled = pf::RAII([this] {
        if (getEnabled() == Enabled::No) { ImGui::EndDisabled(); }
      });
      setHovered(ImGui::IsWindowHovered());
      Collapsible::setCollapsed(ImGui::IsWindowCollapsed());
      updateFocused(ImGui::IsWindowFocused());
      updatePosition(Position{ImGui::GetWindowPos()});
      if (!isCollapsed()) {
        if (hasMenuBar()) { menuBar->render(); }
        std::ranges::for_each(getChildren(), &Renderable::render);
      }
    }
  }
  if (!isNotClosed) {
    notifyClosed();
    setVisibility(Visibility::Invisible);
  }
}

WindowMenuBar &Window::getMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<WindowMenuBar>("menu_bar"); }
  return *menuBar;
}

bool Window::hasMenuBar() const { return menuBar != nullptr; }

void Window::removeMenuBar() { menuBar = nullptr; }

void Window::setSize(const Size &newSize) {
  sizeDirty = true;
  Resizable::setSize(newSize);
}

void Window::render() {
  if (getVisibility() == Visibility::Visible) {
    if (font != nullptr) { ImGui::PushFont(font); }
    ImGui::SetNextWindowSizeConstraints(static_cast<ImVec2>(minSizeConstraint.value_or(Size{0, 0})),
                                        static_cast<ImVec2>(maxSizeConstraint.value_or(Size{
                                            std::numeric_limits<float>::max(), std::numeric_limits<float>::max()})));
    if (dockInto.has_value()) {
      ImGui::SetNextWindowDockID(*dockInto);
      dockInto = std::nullopt;
    }
    renderImpl();
    if (font != nullptr) { ImGui::PopFont(); }
  }
}

void Window::setCollapsed(bool collapse) {
  ImGui::SetWindowCollapsed(idLabel.c_str(), collapse);
  Collapsible::setCollapsed(collapse);
}

void Window::setFocus() {
  ImGui::SetWindowFocus(idLabel.c_str());
  Focusable::setFocus();
}

void Window::setPosition(Position pos) {
  positionDirty = true;
  Positionable::setPosition(pos);
}

bool Window::isUserResizable() const { return userResizable; }

void Window::setUserResizable(bool resizable) { userResizable = resizable; }

bool Window::isUserMovable() const { return userMovable; }

void Window::setUserMovable(bool movable) { userMovable = movable; }

bool Window::isAutoResize() const { return autoResizeToContent; }

void Window::setAutoResize(bool autoResize) { autoResizeToContent = autoResize; }

bool Window::isHorizontalScrollEnabled() const { return enableHorizontalScroll; }

void Window::setHorizontalScrollEnabled(bool horizontalScroll) { enableHorizontalScroll = horizontalScroll; }

bool Window::isDisplayDot() const { return displayDot; }

void Window::setDisplayDot(bool display) { displayDot = display; }

ImGuiWindowFlags Window::createWindowFlags() {
  ImGuiWindowFlags result = hasMenuBar() ? ImGuiWindowFlags_MenuBar : ImGuiWindowFlags{};
  if (!isCollapsible()) { result |= ImGuiWindowFlags_NoCollapse; }
  if (!titleBarVisible) { result |= ImGuiWindowFlags_NoTitleBar; }
  if (!isDockArea) { result |= ImGuiWindowFlags_NoDocking; }
  if (!userResizable) { result |= ImGuiWindowFlags_NoResize; }
  if (!userMovable) { result |= ImGuiWindowFlags_NoMove; }
  if (autoResizeToContent) { result |= ImGuiWindowFlags_AlwaysAutoResize; }
  if (enableHorizontalScroll) { result |= ImGuiWindowFlags_HorizontalScrollbar; }
  if (displayDot) { result |= ImGuiWindowFlags_UnsavedDocument; }
  if (stayInBackground) { result |= ImGuiWindowFlags_NoBringToFrontOnFocus; }
  return result;
}

const std::optional<Size> &Window::getMinSizeConstraint() const { return minSizeConstraint; }

void Window::setMinSizeConstraint(const Size &newSizeConstraint) { minSizeConstraint = newSizeConstraint; }

void Window::cancelSizeConstraint() {
  cancelMinSizeConstraint();
  cancelMaxSizeConstraint();
}

const std::optional<Size> &Window::getMaxSizeConstraint() const { return maxSizeConstraint; }

void Window::cancelMinSizeConstraint() { minSizeConstraint = std::nullopt; }

void Window::cancelMaxSizeConstraint() { maxSizeConstraint = std::nullopt; }

void Window::setMaxSizeConstraint(const Size &newSizeConstraint) { maxSizeConstraint = newSizeConstraint; }

void Window::setFont(ImFont *fontPtr) { font = fontPtr; }

bool Window::isDockable() const { return isDockArea; }

void Window::setIsDockable(bool dockable) { isDockArea = dockable; }

bool Window::isDocked() const { return isWindowDocked; }

void Window::moveToDock(DockSpace::Id dockSpaceId) { dockInto = dockSpaceId; }

std::vector<Renderable *> Window::getRenderables() {
  auto result = ElementContainer::getRenderables();
  if (menuBar != nullptr) { result.emplace_back(menuBar.get()); }
  return result;
}

bool Window::isTitleBarVisible() const { return titleBarVisible; }

void Window::setTitleBarVisible(bool visible) { titleBarVisible = visible; }

bool Window::isStayInBackground() const { return stayInBackground; }

void Window::setStayInBackground(bool stay) { stayInBackground = stay; }

void Window::refreshIdLabel() { idLabel = fmt::format("{}##{}", getLabel(), getName()); }

}  // namespace pf::ui::ig
