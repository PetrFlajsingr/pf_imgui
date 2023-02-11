//
// Created by xflajs00 on 06.05.2022.
//

#include "InputDialogBuilder.h"
#include <pf_imgui/managers/DialogManager.h>

namespace pf::ui::ig {

InputDialogBuilder::InputDialogBuilder(DialogManager *parent) : dialogManager(parent) {}

InputDialogBuilder &InputDialogBuilder::title(std::string dialogTitle) {
  title_ = std::move(dialogTitle);
  return *this;
}

InputDialogBuilder &InputDialogBuilder::size(Size dialogSize) {
  size_ = dialogSize;
  return *this;
}

InputDialogBuilder &InputDialogBuilder::message(std::string dialogMessage) {
  message_ = std::move(dialogMessage);
  return *this;
}

void InputDialogBuilder::build() {
  using namespace std::string_literals;
  dialogManager->addInputDialog(std::make_unique<InputDialog>("InputDialog_"s + std::to_string(IdCounter++), title_, size_, message_,
                                                              std::move(onInput_), std::move(onCancel_)));
}

}  // namespace pf::ui::ig