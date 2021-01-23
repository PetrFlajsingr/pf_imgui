//
// Created by petr on 11/1/20.
//

#include "MenuBars.h"
#include <algorithm>
#include <imgui.h>
#include <pf_common/Visitor.h>
#include <utility>

namespace pf::ui::ig {

MenuItem::MenuItem(const std::string &elementName, const std::string &label)
    : Element(elementName), Labellable(label) {}

void MenuItem::renderImpl() {
  if (ImGui::MenuItem(getLabel().c_str())) { notifyOnClick(); }
}

void SubMenu::renderImpl() {
  if (ImGui::BeginMenu(getLabel().c_str())) {
    renderItems();
    ImGui::EndMenu();
  }
}

SubMenu::SubMenu(const std::string &elementName, const std::string &label)
    : Element(elementName), Labellable(label) {}

WindowMenuBar::WindowMenuBar(const std::string &elementName) : Element(elementName) {}

void WindowMenuBar::renderImpl() {
  ImGui::BeginMenuBar();
  renderItems();
  ImGui::EndMenuBar();
}
AppMenuBar::AppMenuBar(const std::string &elementName) : Element(elementName) {}

void AppMenuBar::renderImpl() {
  ImGui::BeginMainMenuBar();
  renderItems();
  ImGui::EndMainMenuBar();
}

SubMenu &MenuContainer::addSubmenu(const std::string &name, const std::string &caption) {
  auto newSubMenu = std::make_unique<SubMenu>(name, caption);
  const auto ptr = newSubMenu.get();
  items.emplace_back(std::move(newSubMenu));
  return *ptr;
}

MenuItem &MenuContainer::addItem(const std::string &name, const std::string &caption) {
  auto newItem = std::make_unique<MenuItem>(name, caption);
  const auto ptr = newItem.get();
  items.emplace_back(std::move(newItem));
  return *ptr;
}

void MenuContainer::removeItem(const std::string &name) {
  if (const auto iter = std::ranges::find_if(items, [name](auto &item) { return item->getName() == name; });
      iter != items.end()) {
    items.erase(iter);
  }
}

void MenuContainer::renderItems() {
  std::ranges::for_each(items, [](auto &item) { item->render(); });
}

}// namespace pf::ui::ig
