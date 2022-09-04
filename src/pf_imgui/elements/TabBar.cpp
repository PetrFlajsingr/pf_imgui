//
// Created by petr on 11/2/20.
//

#include "TabBar.h"
#include <algorithm>
#include <imgui.h>
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

TabButton::TabButton(TabButton::Config &&config)
    : ItemElement(std::string{config.name.value}), label(std::string{config.label.value}), flags(*config.mods) {}

TabButton::TabButton(const std::string &elementName, const std::string &labelText, const Flags<TabMod> &mods)
    : ItemElement(elementName), label(labelText), flags(*mods) {}

void TabButton::setMods(const Flags<TabMod> &mods) { flags = *mods; }

void TabButton::renderImpl() {
  if (ImGui::TabItemButton(label->get().c_str(), flags)) { clickEvent.notify(); }
}

void TabButton::notifyClickEvent() { clickEvent.notify(); }

Tab::Tab(Tab::Config &&config)
    : TabButton(std::string{config.name.value}, std::string{config.label.value}, config.mods),
      open(config.closeable ? new bool{true} : nullptr) {}

Tab::Tab(const std::string &elementName, const std::string &labelText, const Flags<TabMod> &mods, bool closeable)
    : TabButton(elementName, labelText, mods), open(closeable ? new bool{true} : nullptr) {
  selected.addListener([this](auto newSelected) {
    if (newSelected) { notifyClickEvent(); }
    setSelectedInNextFrame = newSelected;
  });
}

Tab::Tab(const std::string &elementName, const std::string &labelText, bool closeable)
    : Tab(elementName, labelText, Flags<TabMod>{}, closeable) {}

Tab::~Tab() { delete open; }

void Tab::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();

  const auto wasOpen = isOpen();
  const auto frameFlags = flags | (setSelectedInNextFrame ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None);
  *selected.modify() = ImGui::BeginTabItem(label->get().c_str(), open, frameFlags);
  if (*selected) {
    RAII end{ImGui::EndTabItem};
    std::ranges::for_each(getChildren(), &Renderable::render);
  }
  if (open != nullptr && !*open && *open != wasOpen) { closeEvent.notify(); }
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

TabBar::TabBar(TabBar::Config &&config) : ElementWithID(std::string{config.name.value}) {
  setTabListAllowed(config.allowTabList);
}

TabBar::TabBar(const std::string &elementName, bool allowTabList) : ElementWithID(elementName) {
  setTabListAllowed(allowTabList);
}

void TabBar::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::BeginTabBar(getName().c_str(), flags)) {
    RAII end{ImGui::EndTabBar};
    std::ranges::for_each(tabs, [](auto &tab) { tab->render(); });
  }
}

Tab &TabBar::addTab(const std::string &tabName, const std::string &caption, const Flags<TabMod> &mods, bool closeable) {
  tabs.emplace_back(std::make_unique<Tab>(tabName, caption, mods, closeable));
  return dynamic_cast<Tab &>(*tabs.back());
}

Tab &TabBar::addTab(const std::string &tabName, const std::string &caption, bool closeable) {
  return addTab(tabName, caption, Flags<TabMod>{}, closeable);
}

TabButton &TabBar::addTabButton(const std::string &buttonName, const std::string &caption, const Flags<TabMod> &mods) {
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
