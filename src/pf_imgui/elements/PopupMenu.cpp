//
// Created by petr on 6/10/21.
//

#include "PopupMenu.h"

namespace pf::ui::ig {

PopupMenu::PopupMenu(const std::string &name) : Element(name) {}

void PopupMenu::open() { opened = true; }

void PopupMenu::close() {
  opened = false;
  firstRender = true;
}
void PopupMenu::renderImpl() {
  if (opened) {
    if (firstRender) { ImGui::OpenPopup(getName().c_str()); }
    if (ImGui::BeginPopup(getName().c_str())) {
      renderItems();
      ImGui::EndPopup();
      firstRender = false;
    } else {
      firstRender = true;
    }
  }
}

}// namespace pf::ui::ig