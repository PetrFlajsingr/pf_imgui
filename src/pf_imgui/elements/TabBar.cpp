//
// Created by petr on 11/2/20.
//

#include "TabBar.h"
#include <algorithm>
#include <imgui.h>
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

TabButton::TabButton(TabButton::Config &&config)
    : ItemElement(std::string{config.name.value}), Labellable(std::string{config.label.value}), flags(*config.mods) {}

TabButton::TabButton(const std::string &elementName, const std::string &label, const Flags<TabMod> &mods)
    : ItemElement(elementName), Labellable(label), flags(*mods) {}

void TabButton::renderImpl() {
  if (ImGui::TabItemButton(getLabel().c_str(), flags)) { notifyOnClick(); }
}

void TabButton::setMods(const Flags<TabMod> &mods) { flags = *mods; }

Tab::Tab(Tab::Config &&config)
    : TabButton(std::string{config.name.value}, std::string{config.label.value}, config.mods),
      open(config.closeable ? new bool{true} : nullptr) {}

Tab::Tab(const std::string &elementName, const std::string &label, const Flags<TabMod> &mods, bool closeable)
    : TabButton(elementName, label, mods), open(closeable ? new bool{true} : nullptr) {}

Tab::Tab(const std::string &elementName, const std::string &label, bool closeable)
    : Tab(elementName, label, Flags<TabMod>{}, closeable) {}

Tab::~Tab() { delete open; }

void Tab::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();

  const auto wasOpen = isOpen();
  const auto wasSelected = selected;
  const auto frameFlags = flags | (setSelectedInNextFrame ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None);
  selected = ImGui::BeginTabItem(getLabel().c_str(), open, frameFlags);
  if (selected) {
    RAII end{ImGui::EndTabItem};
    std::ranges::for_each(getChildren(), &Renderable::render);
  }
  if (open != nullptr && *open != wasOpen) { openObservable.notify(*open); }
  if (wasSelected != selected) {
    selectedObservable.notify(selected);
    notifyOnClick();
  }
  setSelectedInNextFrame = false;
}

bool Tab::isOpen() const { return open == nullptr || *open; }

void Tab::setOpen(bool newOpen) {
  if (open != nullptr) { *open = newOpen; }
}

bool Tab::isSelected() const { return selected; }

void Tab::setSelected() { setSelectedInNextFrame = true; }

bool Tab::isDisplayDot() const { return flags & ImGuiTabItemFlags_UnsavedDocument; }

void Tab::setDisplayDot(bool displayDot) {
  if (displayDot) {
    flags |= ImGuiTabItemFlags_UnsavedDocument;
  } else {
    flags &= ~ImGuiTabItemFlags_UnsavedDocument;
  }
}

TabBar::TabBar(TabBar::Config &&config) : Element(std::string{config.name.value}) {
  setTabListAllowed(config.allowTabList);
}

TabBar::TabBar(const std::string &elementName, bool allowTabList) : Element(elementName) {
  setTabListAllowed(allowTabList);
}

void TabBar::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
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
    if (auto t = dynamic_cast<Tab *>(tab.get()); t != nullptr) { return t->isSelected(); }
    return false;
  }));
}

void TabBar::setSelectedTab(std::string_view tabName) {
  if (const auto iter = std::ranges::find_if(tabs, [tabName](auto &tab) { return tab->getName() == tabName; });
      iter != tabs.end()) {
    if (auto t = dynamic_cast<Tab *>((*iter).get()); t != nullptr) { t->setSelected(); }
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
