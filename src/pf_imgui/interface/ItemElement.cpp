//
// Created by petr on 1/23/21.
//

#include "ItemElement.h"
#include "src/pf_imgui/elements/Text.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

ItemElement::ItemElement(const std::string &elementName) : Element(elementName) {}

ItemElement::ItemElement(ItemElement &&other) noexcept : Element(std::move(other)), tooltip(std::move(other.tooltip)) {}

ItemElement &ItemElement::operator=(ItemElement &&other) noexcept {
  tooltip = std::move(other.tooltip);
  Element::operator=(std::move(other));
  return *this;
}

void ItemElement::render() {
  Element::render();
  setHovered(ImGui::IsItemHovered());
  updateFocused(ImGui::IsItemFocused());

  if (isHovered()) {
    auto newMousePos = ImGui::GetMousePos() - ImGui::GetItemRectMin();
    if (newMousePos.x != lastMousePosition.x && newMousePos.y != lastMousePosition.y) {
      lastMousePosition = newMousePos;
      mousePositionObservable.notify(lastMousePosition);
    }
  }

  if (tooltip != nullptr) {
    if (getVisibility() == Visibility::Visible && isHovered()) { tooltip->render(); }
  }
  if (popupMenu != nullptr) {
    if (isHovered() && ImGui::GetIO().MouseClicked[1]) { popupMenu->open(); }
    popupMenu->render();
  }
}

bool ItemElement::hasTooltip() const { return tooltip != nullptr; }

void ItemElement::removeTooltip() { tooltip = nullptr; }

void ItemElement::setTooltip(std::string_view text) {
  tooltip = std::make_unique<Tooltip>(getName() + "_tooltip");
  tooltip->createChild<Text>(tooltip->getName() + "_text", std::string(text));
}

Tooltip &ItemElement::createTooltip() {
  tooltip = std::make_unique<Tooltip>(getName() + "_tooltip");
  return *tooltip;
}

void ItemElement::setFocus() {
  ImGui::SetKeyboardFocusHere();
  Focusable::setFocus();
}

Tooltip &ItemElement::getTooltip() {
#ifndef _MSC_VER// TODO: MSVC internal error
  if (tooltip == nullptr) { throw StackTraceException("Tooltip doesn't exist in {}", getName()); }
#endif
  return *tooltip;
}

void ItemElement::setTooltip(std::unique_ptr<Tooltip> &&newTooltip) { tooltip = std::move(newTooltip); }

PopupMenu &ItemElement::createPopupMenu() {
  popupMenu = std::make_unique<PopupMenu>(getName() + "_popup_menu");
  return *popupMenu;
}

PopupMenu &ItemElement::getPopupMenu() {
#ifndef _MSC_VER// TODO: MSVC internal error
  if (tooltip == nullptr) { throw StackTraceException("Popup menu doesn't exist in {}", getName()); }
#endif
  return *popupMenu;
}

bool ItemElement::hasPopupMenu() const { return popupMenu != nullptr; }

}// namespace pf::ui::ig