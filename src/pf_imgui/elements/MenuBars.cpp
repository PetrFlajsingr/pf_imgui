//
// Created by petr on 11/1/20.
//

#include "MenuBars.h"
#include <algorithm>
#include <imgui.h>
#include <pf_common/Visitor.h>

namespace pf::ui::ig {

MenuItem::MenuItem(const std::string &elementName, const std::string &caption)
    : Element(elementName), LabeledElement(elementName, caption) {}

void MenuItem::renderImpl() {
  if (ImGui::MenuItem(getLabel().c_str())) { onClick(); }
}

void SubMenu::renderImpl() {
  if (ImGui::BeginMenu(getLabel().c_str())) {
    renderItems();
    ImGui::EndMenu();
  }
}

SubMenu::SubMenu(const std::string &elementName, const std::string &caption)
    : Element(elementName), LabeledElement(elementName, caption) {}

WindowMenuBar::WindowMenuBar(const std::string &elementName)
    : Element(elementName) {}

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
  items.emplace_back(SubMenu(name, caption));
  return std::get<SubMenu>(items.back());
}

MenuItem &MenuContainer::addItem(const std::string &name, const std::string &caption) {
  items.emplace_back(MenuItem(name, caption));
  return std::get<MenuItem>(items.back());
}
void MenuContainer::removeItem(const std::string &name) {
  if (const auto iter = std::ranges::find_if(
          items,
          [name](auto &item) {
            return std::visit(
                Visitor{[name](SubMenu &item) { return item.getName() == name; },
                        [name](MenuItem &item) { return item.getName() == name; }},
                item);
          });
      iter != items.end()) {
    items.erase(iter);
  }
}
void MenuContainer::renderItems() {
  std::ranges::for_each(items, [](auto &item) {
    std::visit(Visitor{[](MenuItem &item) { item.render(); },
                       [](SubMenu &subMenu) { subMenu.render(); }},
               item);
  });
}
}// namespace pf::ui::ig
