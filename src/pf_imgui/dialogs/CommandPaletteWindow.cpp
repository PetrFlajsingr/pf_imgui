//
// Created by petr.flajsingr on 2/8/2022.
//

#include "CommandPaletteWindow.h"

namespace pf::ui::ig {

CommandPaletteWindow::CommandPaletteWindow(const std::string &name)
    : Renderable(name), context(ImCmd::CreateContext()) {
  setCloseable(true);
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

void CommandPaletteWindow::close() {
  setVisibility(Visibility::Invisible);
  notifyClosed();
}

void CommandPaletteWindow::show() {
  open = true;
  setVisibility(Visibility::Visible);
}

void CommandPaletteWindow::renderImpl() {
  ImCmd::SetCurrentContext(context);
  ImCmd::CommandPaletteWindow(getName().c_str(), &open);
  if (isCloseable() && !open) {
    setVisibility(Visibility::Invisible);
    notifyClosed();
  }
}

}  // namespace pf::ui::ig
