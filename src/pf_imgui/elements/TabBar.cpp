//
// Created by petr on 11/2/20.
//

#include "TabBar.h"
#include <algorithm>
#include <imgui.h>

namespace pf::ui::ig {

TabButton::TabButton(const std::string &elementName, const std::string &label, Flags<TabMod> mods)
    : ItemElement(elementName), Labellable(label), flags(*mods) {}

void TabButton::renderImpl() {
  if (ImGui::TabItemButton(getLabel().c_str(), flags)) { notifyOnClick(); }
}

void TabButton::setMods(const Flags<TabMod> &mods) { flags = *mods; }

Tab::Tab(const std::string &elementName, const std::string &label, const Flags<TabMod> &mods, bool closeable)
    : TabButton(elementName, label, mods), open(closeable ? new bool{true} : nullptr) {}

Tab::Tab(const std::string &elementName, const std::string &label, bool closeable)
    : Tab(elementName, label, Flags<TabMod>{}, closeable) {}

Tab::~Tab() { delete open; }

void Tab::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();

  const auto wasOpen = isOpen();
  const auto wasSelected = selected;
  const auto frameFlags = flags | (setSelectedInNextFrame ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None);
  selected = ImGui::BeginTabItem(getLabel().c_str(), open, frameFlags);
  if (selected) {
    RAII end{[] { ImGui::EndTabItem(); }};
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
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

TabBar::TabBar(const std::string &elementName, bool allowTabList) : Element(elementName) {
  setTabListAllowed(allowTabList);
}

void TabBar::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (ImGui::BeginTabBar(getName().c_str(), flags)) {
    RAII end{[] { ImGui::EndTabBar(); }};
    std::ranges::for_each(tabs, [](auto &tab) { tab->render(); });
  }
}

Tab &TabBar::addTab(const std::string &name, const std::string &caption, const Flags<TabMod> &mods, bool closeable) {
  tabs.emplace_back(std::make_unique<Tab>(name, caption, mods, closeable));
  return dynamic_cast<Tab &>(*tabs.back());
}

Tab &TabBar::addTab(const std::string &name, const std::string &caption, bool closeable) {
  return addTab(name, caption, Flags<TabMod>{}, closeable);
}

TabButton &TabBar::addTabButton(const std::string &name, const std::string &caption, const Flags<TabMod> &mods) {
  tabs.emplace_back(std::make_unique<TabButton>(name, caption, mods));
  return *tabs.back();
}

void TabBar::removeTab(const std::string &name) {
  if (const auto iter = std::ranges::find_if(tabs, [name](const auto &tab) { return tab->getName() == name; });
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

}// namespace pf::ui::ig