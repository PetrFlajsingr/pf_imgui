//
// Created by petr on 1/23/21.
//

#include "ItemElement.h"
#include "../Text.h"
#include <imgui.h>

namespace pf::ui::ig {

ItemElement::ItemElement(const std::string &elementName) : Element(elementName) {}

void ItemElement::render() {
  Element::render();
  hovered = ImGui::IsItemHovered();
  focused = ImGui::IsItemFocused();
  if (tooltip != nullptr) {
    if (getVisibility() == Visibility::Visible && hovered) { tooltip->render(); }
  }
}

bool ItemElement::isFocused() const { return focused; }

bool ItemElement::isHovered() const { return hovered; }

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

}// namespace pf::ui::ig