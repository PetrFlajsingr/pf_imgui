//
// Created by petr.flajsingr on 2/8/2022.
//

#include "CommandPalette.h"

namespace pf::ui::ig {

CommandPalette::CommandPalette(CommandPalette::Config &&config) : CommandPalette(config.name) {}

CommandPalette::CommandPalette(std::string_view elementName)
    : ElementWithID(elementName), context(ImCmd::CreateContext()) {}

CommandPalette::~CommandPalette() { ImCmd::DestroyContext(context); }

void CommandPalette::removeCommand(const std::string &commandName) {
  ImCmd::SetCurrentContext(context);
  ImCmd::RemoveCommand(commandName.c_str());
}

void CommandPalette::moveFocusToSearchBox() {
  ImCmd::SetCurrentContext(context);
  ImCmd::SetNextCommandPaletteSearchBoxFocused();
}

void CommandPalette::setSearchText(std::string_view text) {
  ImCmd::SetCurrentContext(context);
  ImCmd::SetNextCommandPaletteSearch(std::string{text});
}

void CommandPalette::renderImpl() {
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImCmd::CommandPalette(getName().c_str());
  if (ImCmd::IsAnyItemSelected()) { Event_notify(interactEvent); }
}

}  // namespace pf::ui::ig
