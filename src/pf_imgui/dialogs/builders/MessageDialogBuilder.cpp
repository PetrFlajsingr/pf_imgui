//
// Created by xflajs00 on 06.05.2022.
//

#include "MessageDialogBuilder.h"
#include <pf_imgui/managers/DialogManager.h>

namespace pf::ui::ig {

MessageDialogBuilder::MessageDialogBuilder(DialogManager *parent) : dialogManager(parent) {}

MessageDialogBuilder &MessageDialogBuilder::title(std::string dialogTitle) {
  title_ = std::move(dialogTitle);
  return *this;
}

MessageDialogBuilder &MessageDialogBuilder::size(Size dialogSize) {
  size_ = dialogSize;
  return *this;
}

MessageDialogBuilder &MessageDialogBuilder::message(std::string dialogMessage) {
  message_ = std::move(dialogMessage);
  return *this;
}

void MessageDialogBuilder::build() {
  using namespace std::string_literals;
  dialogManager->addMessageDialog(std::make_unique<MessageDialog>("MessageDialog_"s + std::to_string(IdCounter++), title_, size_, message_,
                                                                  buttons_, std::move(onDone_)));
}

}  // namespace pf::ui::ig