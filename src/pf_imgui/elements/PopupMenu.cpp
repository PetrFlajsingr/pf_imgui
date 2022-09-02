//
// Created by petr on 6/10/21.
//

#include "PopupMenu.h"
#include <string>

namespace pf::ui::ig {

PopupMenu::PopupMenu(PopupMenu::Config &&config) : ElementWithID(std::string{config.name.value}) {}

PopupMenu::PopupMenu(const std::string &elementName) : ElementWithID(elementName) {}

void PopupMenu::open() { opened = true; }

bool PopupMenu::isOpen() const { return opened; }

void PopupMenu::close() {
  opened = false;
  firstRender = true;
}

void PopupMenu::renderImpl() {
  if (opened) {
    [[maybe_unused]] auto colorScoped = color.applyScoped();
    [[maybe_unused]] auto styleScoped = style.applyScoped();
    [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
    if (firstRender) { ImGui::OpenPopup(getName().c_str()); }
    if (ImGui::BeginPopup(getName().c_str())) {
      RAII end{ImGui::EndPopup};
      renderItems();
      firstRender = false;
    } else {
      firstRender = true;
      opened = false;
    }
  }
}

}  // namespace pf::ui::ig
