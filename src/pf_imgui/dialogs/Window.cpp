//
// Created by petr on 10/31/20.
//

#include "Window.h"
#include <algorithm>
#include <fmt/format.h>
#include <imgui.h>
#include <limits>
#include <utility>
#include <vector>

namespace pf::ui::ig {

Window::Window(std::string elementName, std::string titleLabel, AllowCollapse allowCollapse, Persistent persistent)
    : Renderable(std::move(elementName)), Collapsible(allowCollapse, persistent), size(Size::Auto()),
      position(Position{-1, -1}), label(std::move(titleLabel)) {
  refreshIdLabel();
  label.addListener([this](auto) { refreshIdLabel(); });
  focused.addListener([this](bool isFocused) {
    if (isFocused) { ImGui::SetWindowFocus(idLabel.c_str()); }
  });
  position.addListener([this](auto) { positionDirty = true; });
  size.addListener([this](auto) { sizeDirty = true; });
}

Window::Window(std::string elementName, std::string titleLabel, Persistent persistent)
    : Window(std::move(elementName), std::move(titleLabel), AllowCollapse::No, persistent) {}

void Window::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto isNotClosed = true;

  if (sizeDirty) {
    sizeDirty = false;
    ImGui::SetNextWindowSize(static_cast<ImVec2>(*size));
  }
  if (positionDirty) {
    positionDirty = false;
    ImGui::SetNextWindowPos(static_cast<ImVec2>(*position));
  }

  RAII endPopup{ImGui::End};
  if (ImGui::Begin(idLabel.c_str(), (isCloseable() ? &isNotClosed : nullptr),
                   flags | (hasMenuBar() ? ImGuiWindowFlags_MenuBar : 0))) {
    isWindowDocked = ImGui::IsWindowDocked();
    if (firstPass) {
      firstPass = false;
      if (*size != Size::Auto()) { sizeDirty = true; }
      if (position->x != -1 && position->y != -1) { positionDirty = true; }  //-V550
    }
    if (getEnabled() == Enabled::No) { ImGui::BeginDisabled(); }
    {
      auto raiiEnabled = pf::RAII([this] {
        if (getEnabled() == Enabled::No) { ImGui::EndDisabled(); }
      });
      *hovered.modify() = ImGui::IsWindowHovered();
      Collapsible::setCollapsed(ImGui::IsWindowCollapsed());
      *focused.modify() = ImGui::IsWindowFocused();
      *position.modify() = Position{ImGui::GetWindowPos()};
      positionDirty = false;
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

WindowMenuBar &Window::createOrGetMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<WindowMenuBar>("menu_bar"); }
  return *menuBar;
}

bool Window::hasMenuBar() const { return menuBar != nullptr; }

void Window::removeMenuBar() { menuBar = nullptr; }

void Window::render() {
  if (getVisibility() == Visibility::Visible) {
    ImGui::SetNextWindowSizeConstraints(static_cast<ImVec2>(minSizeConstraint.value_or(Size{0, 0})),
                                        static_cast<ImVec2>(maxSizeConstraint.value_or(Size{
                                            std::numeric_limits<float>::max(), std::numeric_limits<float>::max()})));
    if (dockInto.has_value()) {
      ImGui::SetNextWindowDockID(*dockInto);
      dockInto = std::nullopt;
    }
    renderImpl();
  }
}

void Window::setCollapsed(bool collapse) {
  ImGui::SetWindowCollapsed(idLabel.c_str(), collapse);
  Collapsible::setCollapsed(collapse);
}

bool Window::isUserResizable() const { return flags & ImGuiWindowFlags_NoResize; }

void Window::setUserResizable(bool resizable) {
  if (resizable) {
    flags &= ~ImGuiWindowFlags_NoResize;
  } else {
    flags |= ImGuiWindowFlags_NoResize;
  }
}

bool Window::isUserMovable() const { return flags & ImGuiWindowFlags_NoMove; }

void Window::setUserMovable(bool movable) {
  if (movable) {
    flags &= ~ImGuiWindowFlags_NoMove;
  } else {
    flags |= ImGuiWindowFlags_NoMove;
  }
}

bool Window::isAutoResize() const { return flags & ImGuiWindowFlags_AlwaysAutoResize; }

void Window::setAutoResize(bool autoResize) {
  if (autoResize) {
    flags |= ImGuiWindowFlags_AlwaysAutoResize;
  } else {
    flags &= ~ImGuiWindowFlags_AlwaysAutoResize;
  }
}

bool Window::isHorizontalScrollEnabled() const { return flags & ImGuiWindowFlags_HorizontalScrollbar; }

void Window::setHorizontalScrollEnabled(bool horizontalScroll) {
  if (horizontalScroll) {
    flags |= ImGuiWindowFlags_HorizontalScrollbar;
  } else {
    flags &= ~ImGuiWindowFlags_HorizontalScrollbar;
  }
}

bool Window::isDisplayDot() const { return flags & ImGuiWindowFlags_UnsavedDocument; }

void Window::setDisplayDot(bool display) {
  if (display) {
    flags |= ImGuiWindowFlags_UnsavedDocument;
  } else {
    flags &= ~ImGuiWindowFlags_UnsavedDocument;
  }
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

bool Window::isDockable() const { return flags & ImGuiWindowFlags_NoDocking; }

void Window::setIsDockable(bool dockable) {
  if (dockable) {
    flags &= ~ImGuiWindowFlags_NoDocking;
  } else {
    flags |= ImGuiWindowFlags_NoDocking;
  }
}

bool Window::isDocked() const { return isWindowDocked; }

void Window::moveToDock(DockSpace::Id dockSpaceId) { dockInto = dockSpaceId; }

std::vector<Renderable *> Window::getRenderables() {
  auto result = ElementContainer::getRenderables();
  if (menuBar != nullptr) { result.emplace_back(menuBar.get()); }
  return result;
}

bool Window::isTitleBarVisible() const { return flags & ImGuiWindowFlags_NoTitleBar; }

void Window::setTitleBarVisible(bool visible) {
  if (visible) {
    flags &= ~ImGuiWindowFlags_NoTitleBar;
  } else {
    flags |= ImGuiWindowFlags_NoTitleBar;
  }
}

bool Window::isStayInBackground() const { return flags & ImGuiWindowFlags_NoBringToFrontOnFocus; }

void Window::setStayInBackground(bool stay) {
  if (stay) {
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  } else {
    flags &= ~ImGuiWindowFlags_NoBringToFrontOnFocus;
  }
}

void Window::refreshIdLabel() { idLabel = fmt::format("{}##{}", label->get(), getName()); }

void Window::setCollapsible(bool newCollapsible) {
  if (newCollapsible) {
    flags &= ~ImGuiWindowFlags_NoCollapse;
  } else {
    flags |= ImGuiWindowFlags_NoCollapse;
  }
  Collapsible::setCollapsible(newCollapsible);
}

}  // namespace pf::ui::ig
