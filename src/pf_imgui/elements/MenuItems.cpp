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

MenuItem::MenuItem(std::string_view elementName) : ElementWithID(elementName) {}

bool MenuItem::isCloseMenuOnInteract() const { return closeOnInteract; }

void MenuItem::setCloseOnInteract(bool close) { closeOnInteract = close; }

void MenuItem::render() {
  ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, !closeOnInteract);
  ElementWithID::render();
  ImGui::PopItemFlag();
}

MenuButtonItem::MenuButtonItem(MenuButtonItem::Config &&config)
    : MenuItem(std::string{config.name.value}), label(std::string{config.label.value}) {}

MenuButtonItem::MenuButtonItem(std::string_view elementName, std::string_view labelText)
    : MenuItem(elementName), label(std::string{labelText}) {}

void MenuButtonItem::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto scopedFont = font.applyScopedIfNotDefault();
  if (ImGui::MenuItem(label->get().c_str(), nullptr)) { clickEvent.notify(); }
}

void SubMenu::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto scopedFont = font.applyScopedIfNotDefault();
  if (ImGui::BeginMenu(label->get().c_str())) {
    RAII end{ImGui::EndMenu};
    renderItems();
  }
}

SubMenu::SubMenu(SubMenu::Config &&config)
    : MenuItem(std::string{config.name.value}), label(std::string{config.label.value}) {}

SubMenu::SubMenu(std::string_view elementName, std::string_view labelText)
    : MenuItem(elementName), label(std::string{labelText}) {}

SubMenu &MenuContainer::addSubmenu(std::string_view name, std::string_view caption) {
  return addMenuItem<SubMenu>(name, caption);
}

MenuButtonItem &MenuContainer::addButtonItem(std::string_view name, std::string_view caption) {
  return addMenuItem<MenuButtonItem>(name, caption);
}

MenuCheckboxItem &MenuContainer::addCheckboxItem(std::string_view name, std::string_view caption, bool value,
                                                 Persistent persistent) {
  return addMenuItem<MenuCheckboxItem>(name, caption, value, persistent);
}

MenuSeparatorItem &MenuContainer::addSeparator(std::string_view name) { return addMenuItem<MenuSeparatorItem>(name); }

void MenuContainer::renderItems() { std::ranges::for_each(getChildren(), &Renderable::render); }

MenuCheckboxItem::MenuCheckboxItem(MenuCheckboxItem::Config &&config)
    : MenuItem(config.name.value), Savable(config.persistent ? Persistent::Yes : Persistent::No),
      label(std::string{config.label.value}), checked(config.checked) {}

MenuCheckboxItem::MenuCheckboxItem(std::string_view elementName, std::string_view labelText, bool initialValue,
                                   Persistent persistent)
    : MenuItem(elementName), Savable(persistent), label(std::string{labelText}), checked(initialValue) {}

void MenuCheckboxItem::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto scopedFont = font.applyScopedIfNotDefault();
  if (ImGui::MenuItem(label->get().c_str(), nullptr, &checked.value)) { checked.triggerListeners(); }
}

toml::table MenuCheckboxItem::toToml() const { return toml::table{{"checked", getValue()}}; }

void MenuCheckboxItem::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("checked"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { *checked.modify() = *newVal; }
  }
}

void MenuCheckboxItem::setValue(const bool &newValue) { *checked.modify() = newValue; }

const bool &MenuCheckboxItem::getValue() const { return *checked; }

Subscription MenuCheckboxItem::addValueListenerImpl(std::function<void(const bool &)> listener) {
  return checked.addListener(std::move(listener));
}

MenuSeparatorItem::MenuSeparatorItem(MenuSeparatorItem::Config &&config) : MenuItem(std::string{config.name.value}) {}

MenuSeparatorItem::MenuSeparatorItem(std::string_view elementName) : MenuItem(elementName) {}

void MenuSeparatorItem::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  ImGui::Separator();
}

}  // namespace pf::ui::ig
