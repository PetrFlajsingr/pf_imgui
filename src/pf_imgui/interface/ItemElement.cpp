//
// Created by petr on 1/23/21.
//

#include "ItemElement.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_imgui/elements/Text.h>
#include <utility>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/elements/Tooltip.h>

namespace pf::ui::ig {

ItemElement::ItemElement(const std::string &elementName) : ElementWithID(elementName) {}

ItemElement::~ItemElement() = default;

ItemElement::ItemElement(ItemElement &&other) noexcept
    : ElementWithID(std::move(other)), tooltip(std::move(other.tooltip)) {}

ItemElement &ItemElement::operator=(ItemElement &&other) noexcept {
  tooltip = std::move(other.tooltip);
  ElementWithID::operator=(std::move(other));
  return *this;
}

void ItemElement::render() {
  ElementWithID::render();
  setHovered(ImGui::IsItemHovered());
  setFocused(ImGui::IsItemFocused());

  if (*hovered) {
    auto newMousePos = ImGui::GetMousePos() - ImGui::GetItemRectMin();
    if (newMousePos.x != lastMousePosition.x && newMousePos.y != lastMousePosition.y) {  //-V550
      lastMousePosition = newMousePos;
      mousePositionObservable.notify(Position{lastMousePosition});
    }
  }

  if (tooltip != nullptr) {
    if (*visibility == Visibility::Visible && *hovered) { tooltip->render(); }
  }
  if (popupMenu != nullptr) {
    if (*hovered && ImGui::GetIO().MouseClicked[1]) { popupMenu->open(); }
    popupMenu->render();
  }
}

Tooltip &ItemElement::createOrGetTooltip() {
  if (tooltip != nullptr) { return *tooltip; }
  tooltip = std::make_unique<Tooltip>("tooltip");
  return *tooltip;
}

bool ItemElement::hasTooltip() const { return tooltip != nullptr; }

void ItemElement::removeTooltip() { tooltip = nullptr; }

void ItemElement::setTooltip(std::string_view text) {
  tooltip = std::make_unique<Tooltip>("tooltip");
  tooltip->createChild<Text>("text", std::string(text));
}

void ItemElement::setTooltip(std::unique_ptr<Tooltip> &&newTooltip) { tooltip = std::move(newTooltip); }

PopupMenu &ItemElement::createOrGetPopupMenu() {
  if (popupMenu != nullptr) { return *popupMenu; }
  popupMenu = std::make_unique<PopupMenu>("popup_menu");
  return *popupMenu;
}

bool ItemElement::hasPopupMenu() const { return popupMenu != nullptr; }

void ItemElement::removePopupMenu() { popupMenu = nullptr; }

void ItemElement::setHovered(bool newHovered) {
  *hovered.modify() = newHovered;
}

void ItemElement::setFocused(bool newFocused) {
  *focused.modify() = newFocused;
}

}  // namespace pf::ui::ig
