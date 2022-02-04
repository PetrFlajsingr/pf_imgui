//
// Created by petr on 6/10/21.
//

#include "PopupMenu.h"

namespace pf::ui::ig {

PopupMenu::PopupMenu(const std::string &name) : Element(name) {}

void PopupMenu::open() { opened = true; }

bool PopupMenu::isOpen() const { return opened; }

void PopupMenu::close() {
  opened = false;
  firstRender = true;
}

void PopupMenu::renderImpl() {
  if (opened) {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    if (firstRender) { ImGui::OpenPopup(getName().c_str()); }
    if (ImGui::BeginPopup(getName().c_str())) {
      RAII end{[] { ImGui::EndPopup(); }};
      renderItems();
      firstRender = false;
    } else {
      firstRender = true;
      opened = false;
    }
  }
}

}// namespace pf::ui::ig