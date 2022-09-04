//
// Created by petr.flajsingr on 2/8/2022.
//

#include "CommandPaletteWindow.h"

namespace pf::ui::ig {

CommandPaletteWindow::CommandPaletteWindow(const std::string &elementName)
    : Renderable(elementName), context(ImCmd::CreateContext()) {
}

CommandPaletteWindow::~CommandPaletteWindow() { ImCmd::DestroyContext(context); }

void CommandPaletteWindow::removeCommand(const std::string &commandName) {
  ImCmd::SetCurrentContext(context);
  ImCmd::RemoveCommand(commandName.c_str());
}

void CommandPaletteWindow::moveFocusToSearchBox() {
  ImCmd::SetCurrentContext(context);
  ImCmd::SetNextCommandPaletteSearchBoxFocused();
}

void CommandPaletteWindow::setSearchText(const std::string &text) {
  ImCmd::SetCurrentContext(context);
  ImCmd::SetNextCommandPaletteSearch(text);
}

void CommandPaletteWindow::renderImpl() {
  ImCmd::SetCurrentContext(context);
  bool open = true;
  ImCmd::CommandPaletteWindow(getName().c_str(), &open);
  if (!open) {
    *visibility.modify() = Visibility::Invisible;
  }
}

}  // namespace pf::ui::ig
