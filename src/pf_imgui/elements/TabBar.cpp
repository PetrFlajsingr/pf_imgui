//
// Created by petr on 11/2/20.
//

#include "TabBar.h"
#include <algorithm>
#include <imgui.h>
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

TabButton::TabButton(TabButton::Config &&config) : TabButton(config.name, config.label, config.mods) {}

TabButton::TabButton(std::string_view elementName, std::string_view labelText, Flags<TabMod> mods)
    : ItemElement(elementName), label(std::string{labelText}), flags(*mods) {}

void TabButton::setMods(const Flags<TabMod> &mods) { flags = *mods; }

void TabButton::renderImpl() {
  if (ImGui::TabItemButton(label->get().c_str(), flags)) { Event_notify(clickEvent); }
}

void TabButton::notifyClickEvent() { Event_notify(clickEvent); }

Tab::Tab(Tab::Config &&config) : Tab(config.name, config.label, config.mods, config.closeable) {}

Tab::Tab(std::string_view elementName, std::string_view labelText, Flags<TabMod> mods, bool closeable)
    : TabButton(elementName, labelText, mods), open(closeable ? new bool{true} : nullptr) {
  selected.addListener([this](auto newSelected) {
    if (newSelected) { notifyClickEvent(); }
    setSelectedInNextFrame = newSelected;
  });
}

void Tab::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();

  const auto wasOpen = isOpen();
  const auto frameFlags = flags | (setSelectedInNextFrame ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None);
  *selected.modify() = ImGui::BeginTabItem(label->get().c_str(), open.get(), frameFlags);
  if (*selected) {
    ScopeExit end{&ImGui::EndTabItem};
    std::ranges::for_each(getChildren(), &Renderable::render);
  }
  if (open != nullptr && !*open && *open != wasOpen) { Event_notify(closeEvent); }
  setSelectedInNextFrame = false;
}

bool Tab::isOpen() const { return open == nullptr || *open; }

void Tab::setOpen(bool newOpen) {
  if (open != nullptr) { *open = newOpen; }
}

bool Tab::isDisplayDot() const { return flags & ImGuiTabItemFlags_UnsavedDocument; }

void Tab::setDisplayDot(bool displayDot) {
  if (displayDot) {
    flags |= ImGuiTabItemFlags_UnsavedDocument;
  } else {
    flags &= ~ImGuiTabItemFlags_UnsavedDocument;
  }
}

TabBar::TabBar(TabBar::Config &&config) : TabBar(config.name, config.allowTabList) {}

TabBar::TabBar(std::string_view elementName, bool allowTabList) : ElementWithID(elementName) {
  setTabListAllowed(allowTabList);
}

void TabBar::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::BeginTabBar(getName().c_str(), flags)) {
    ScopeExit end{&ImGui::EndTabBar};
    std::ranges::for_each(tabs, [](auto &tab) { tab->render(); });
  }
}

Tab &TabBar::addTab(std::string_view tabName, std::string_view caption, Flags<TabMod> mods, bool closeable) {
  tabs.emplace_back(std::make_unique<Tab>(tabName, caption, mods, closeable));
  return dynamic_cast<Tab &>(*tabs.back());
}

Tab &TabBar::addTab(std::string_view tabName, std::string_view caption, bool closeable) {
  return addTab(tabName, caption, Flags<TabMod>{}, closeable);
}

TabButton &TabBar::addTabButton(std::string_view buttonName, std::string_view caption, Flags<TabMod> mods) {
  tabs.emplace_back(std::make_unique<TabButton>(buttonName, caption, mods));
  return *tabs.back();
}

void TabBar::removeTab(const std::string &tabName) {
  if (const auto iter = std::ranges::find_if(tabs, [tabName](const auto &tab) { return tab->getName() == tabName; });
      iter != tabs.cend()) {
    tabs.erase(iter);
  }
}

Tab &TabBar::getSelectedTab() {
  return dynamic_cast<Tab &>(**std::ranges::find_if(tabs, [](const auto &tab) {
    if (auto t = dynamic_cast<Tab *>(tab.get()); t != nullptr) { return *t->selected; }
    return false;
  }));
}

void TabBar::setSelectedTab(std::string_view tabName) {
  if (const auto iter = std::ranges::find_if(tabs, [tabName](auto &tab) { return tab->getName() == tabName; });
      iter != tabs.end()) {
    if (auto t = dynamic_cast<Tab *>((*iter).get()); t != nullptr) { *t->selected.modify() = true; }
  }
}

bool TabBar::isTabListAllowed() const { return flags & ImGuiTabBarFlags_TabListPopupButton; }

void TabBar::setTabListAllowed(bool listAllowed) {
  if (listAllowed) {
    flags |= ImGuiTabBarFlags_TabListPopupButton;
  } else {
    flags &= ~ImGuiTabBarFlags_TabListPopupButton;
  }
}

bool TabBar::isReorderable() const { return flags & ImGuiTabBarFlags_Reorderable; }

void TabBar::setReorderable(bool reorderable) {
  if (reorderable) {
    flags |= ImGuiTabBarFlags_Reorderable;
  } else {
    flags &= ~ImGuiTabBarFlags_Reorderable;
  }
}

bool TabBar::isCloseMidMouseButton() const { return flags & ImGuiTabBarFlags_NoCloseWithMiddleMouseButton; }

void TabBar::setCloseMidMouseButton(bool allowedClose) {
  if (allowedClose) {
    flags |= ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
  } else {
    flags &= ~ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
  }
}

Flags<TabBar::ResizePolicy> TabBar::getResizePolicy() const {
  return static_cast<ResizePolicy>(flags & ImGuiTabBarFlags_FittingPolicyMask_);
}

void TabBar::setResizePolicy(const Flags<TabBar::ResizePolicy> &newPolicy) {
  flags = (flags & ~ImGuiTabBarFlags_FittingPolicyMask_) | *newPolicy;
}

std::vector<Renderable *> TabBar::getRenderables() {
  return tabs | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); }) | ranges::to_vector;
}

}  // namespace pf::ui::ig
