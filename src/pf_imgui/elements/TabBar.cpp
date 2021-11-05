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

Tab::~Tab() { delete open; }

void Tab::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();

  const auto wasOpen = isOpen();
  const auto wasSelected = selected;
  const auto frameFlags = flags | (setSelectedInNextFrame ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None);
  selected = ImGui::BeginTabItem(getLabel().c_str(), open, frameFlags);
  if (selected) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
    ImGui::EndTabItem();
  }
  if (open != nullptr && *open != wasOpen) { openObservable.notify(*open); }
  if (wasSelected != selected) {
    selectedObservable.notify(selected);
    notifyOnClick();
  }
  setSelectedInNextFrame = false;
}

bool Tab::isOpen() const { return open == nullptr || *open; }

void Tab::setOpen() {
  if (open != nullptr) { *open = true; }
}

bool Tab::isSelected() const { return selected; }

void Tab::setSelected() { setSelectedInNextFrame = true; }

TabBar::TabBar(const std::string &elementName, bool allowTabList)
    : Element(elementName), tabListAllowed(allowTabList) {}

void TabBar::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto flags = tabListAllowed ? ImGuiTabBarFlags_TabListPopupButton : ImGuiTabBarFlags{};
  if (ImGui::BeginTabBar(getName().c_str(), flags)) {
    std::ranges::for_each(tabs, [](auto &tab) { tab->render(); });
    ImGui::EndTabBar();
  }
}

Tab &TabBar::addTab(const std::string &name, const std::string &caption, const Flags<TabMod> &mods, bool closeable) {
  tabs.emplace_back(std::make_unique<Tab>(name, caption, mods, closeable));
  return dynamic_cast<Tab &>(*tabs.back());
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

bool TabBar::isTabListAllowed() const { return tabListAllowed; }

void TabBar::setTabListAllowed(bool listAllowed) { tabListAllowed = listAllowed; }

std::vector<Renderable *> TabBar::getRenderables() {
  return tabs | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); }) | ranges::to_vector;
}

}// namespace pf::ui::ig