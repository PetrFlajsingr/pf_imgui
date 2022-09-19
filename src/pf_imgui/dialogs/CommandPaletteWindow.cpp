//
// Created by petr.flajsingr on 2/8/2022.
//

#include "CommandPaletteWindow.h"
#include <imcmd_command_palette.h>

namespace pf::ui::ig {

CommandPaletteWindow::CommandPaletteWindow(std::string_view elementName)
    : Renderable(elementName), context(ImCmd::CreateContext()) {}

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
  if (!open) { *visibility.modify() = Visibility::Invisible; }
}

void CommandPaletteWindow::addCommandImpl(std::string commandName, std::function<void()> callback) {
  ImCmd::SetCurrentContext(context);
  auto command = ImCmd::Command{};
  command.Name = std::move(commandName);
  command.InitialCallback = std::forward<decltype(callback)>(callback);
  ImCmd::AddCommand(std::move(command));
}

}  // namespace pf::ui::ig
