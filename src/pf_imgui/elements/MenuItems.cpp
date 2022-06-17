//
// Created by petr on 6/10/21.
//

#include "MenuItems.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

MenuItem::MenuItem(const std::string &name) : ElementWithID(name) {}

bool MenuItem::isCloseMenuOnInteract() const { return closeOnInteract; }

void MenuItem::setCloseOnInteract(bool close) { closeOnInteract = close; }

void MenuItem::render() {
  ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, !closeOnInteract);
  ElementWithID::render();
  ImGui::PopItemFlag();
}

MenuButtonItem::MenuButtonItem(MenuButtonItem::Config &&config)
    : MenuItem(std::string{config.name.value}), Labellable(std::string{config.label.value}) {}

MenuButtonItem::MenuButtonItem(const std::string &elementName, const std::string &label)
    : MenuItem(elementName), Labellable(label) {}

void MenuButtonItem::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  if (ImGui::MenuItem(getLabel().c_str(), nullptr)) { notifyOnClick(); }
}

void SubMenu::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  if (ImGui::BeginMenu(getLabel().c_str())) {
    RAII end{ImGui::EndMenu};
    renderItems();
  }
}

SubMenu::SubMenu(SubMenu::Config &&config)
    : MenuItem(std::string{config.name.value}), Labellable(std::string{config.label.value}) {}

SubMenu::SubMenu(const std::string &elementName, const std::string &label) : MenuItem(elementName), Labellable(label) {}

SubMenu &MenuContainer::addSubmenu(const std::string &name, const std::string &caption) {
  return addMenuItem<SubMenu>(name, caption);
}

MenuButtonItem &MenuContainer::addButtonItem(const std::string &name, const std::string &caption) {
  return addMenuItem<MenuButtonItem>(name, caption);
}

MenuCheckboxItem &MenuContainer::addCheckboxItem(const std::string &name, const std::string &caption, bool value,
                                                 Persistent persistent) {
  return addMenuItem<MenuCheckboxItem>(name, caption, value, persistent);
}

MenuSeparatorItem &MenuContainer::addSeparator(const std::string &name) { return addMenuItem<MenuSeparatorItem>(name); }

void MenuContainer::renderItems() { std::ranges::for_each(getChildren(), &Renderable::render); }

MenuCheckboxItem::MenuCheckboxItem(MenuCheckboxItem::Config &&config)
    : MenuItem(std::string{config.name.value}), Labellable(std::string{config.label.value}),
      ValueObservable(config.checked), Savable(config.persistent ? Persistent::Yes : Persistent::No) {}

MenuCheckboxItem::MenuCheckboxItem(const std::string &elementName, const std::string &label, bool value,
                                   Persistent persistent)
    : MenuItem(elementName), Labellable(label), ValueObservable(value), Savable(persistent) {}

void MenuCheckboxItem::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  if (ImGui::MenuItem(getLabel().c_str(), nullptr, getValueAddress())) { notifyValueChanged(); }
}

toml::table MenuCheckboxItem::toToml() const { return toml::table{{"checked", getValue()}}; }

void MenuCheckboxItem::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("checked"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { setValueAndNotifyIfChanged(*newVal); }
  }
}

MenuSeparatorItem::MenuSeparatorItem(MenuSeparatorItem::Config &&config) : MenuItem(std::string{config.name.value}) {}

MenuSeparatorItem::MenuSeparatorItem(const std::string &elementName) : MenuItem(elementName) {}

void MenuSeparatorItem::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  ImGui::Separator();
}

}  // namespace pf::ui::ig
