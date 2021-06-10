//
// Created by petr on 6/10/21.
//

#include "MenuItems.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace pf::ui::ig {

MenuItem::MenuItem(const std::string &name) : Element(name) {}

bool MenuItem::isCloseMenuOnInteract() const { return closeOnInteract; }

void MenuItem::setCloseOnInteract(bool close) { closeOnInteract = close; }

void MenuItem::render() {
  ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, !closeOnInteract);
  Element::render();
  ImGui::PopItemFlag();
}

MenuButtonItem::MenuButtonItem(const std::string &elementName, const std::string &label)
    : MenuItem(elementName), Labellable(label) {}

void MenuButtonItem::renderImpl() {
  if (ImGui::MenuItem(getLabel().c_str(), nullptr)) { notifyOnClick(); }
}

void SubMenu::renderImpl() {
  if (ImGui::BeginMenu(getLabel().c_str())) {
    renderItems();
    ImGui::EndMenu();
  }
}

SubMenu::SubMenu(const std::string &elementName, const std::string &label) : MenuItem(elementName), Labellable(label) {}

SubMenu &MenuContainer::addSubmenu(const std::string &name, const std::string &caption) {
  return addItem<SubMenu>(name, caption);
}

MenuButtonItem &MenuContainer::addButtonItem(const std::string &name, const std::string &caption) {
  return addItem<MenuButtonItem>(name, caption);
}

MenuCheckboxItem &MenuContainer::addCheckboxItem(const std::string &name, const std::string &caption, bool value) {
  return addItem<MenuCheckboxItem>(name, caption, value);
}

MenuSeparatorItem &MenuContainer::addSeparator(const std::string &name) { return addItem<MenuSeparatorItem>(name); }

void MenuContainer::removeItem(const std::string &name) {
  if (const auto iter = std::ranges::find_if(items, [name](auto &item) { return item->getName() == name; });
      iter != items.end()) {
    items.erase(iter);
  }
}
void MenuContainer::renderItems() {
  std::ranges::for_each(items, [](auto &item) { item->render(); });
}

MenuCheckboxItem::MenuCheckboxItem(const std::string &elementName, const std::string &label, bool value,
                                   Persistent persistent)
    : MenuItem(elementName), Labellable(label), ValueObservable(value), Savable(persistent) {}

void MenuCheckboxItem::renderImpl() {
  if (ImGui::MenuItem(getLabel().c_str(), nullptr, getValueAddress())) { notifyValueChanged(); }
}
void MenuCheckboxItem::unserialize_impl(const toml::table &src) {
  setValueAndNotifyIfChanged(*src["checked"].value<bool>());
}
toml::table MenuCheckboxItem::serialize_impl() { return toml::table{{{"checked", getValue()}}}; }

MenuSeparatorItem::MenuSeparatorItem(const std::string &elementName) : MenuItem(elementName) {}

void MenuSeparatorItem::renderImpl() { ImGui::Separator(); }

}// namespace pf::ui::ig
