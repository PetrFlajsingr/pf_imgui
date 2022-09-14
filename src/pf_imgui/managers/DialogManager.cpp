//
// Created by xflajs00 on 05.05.2022.
//

#include "DialogManager.h"

namespace pf::ui::ig {

FileDialogBuilder DialogManager::buildFileDialog(FileDialogType type) { return FileDialogBuilder(this, type); }

void DialogManager::addFileDialog(std::unique_ptr<FileDialog> &&dialog) {
  auto &dialogRef = fileDialogs.emplace_back(std::move(dialog));
  if (fileDialogBookmark.has_value()) { dialogRef->deserializeBookmark(*fileDialogBookmark); }
}

ModalDialog &DialogManager::createDialog(std::string_view elementName, std::string_view caption) {
  auto dialog = std::make_unique<ModalDialog>(elementName, caption);
  const auto ptr = dialog.get();
  dialogs.emplace_back(std::move(dialog));
  return *ptr;
}

MessageDialogBuilder DialogManager::buildMessageDialog() { return MessageDialogBuilder(this); }

void DialogManager::addMessageDialog(std::unique_ptr<MessageDialog> &&dialog) {
  dialogs.emplace_back(std::move(dialog));
}

InputDialogBuilder DialogManager::buildInputDialog() { return InputDialogBuilder(this); }

void DialogManager::addInputDialog(std::unique_ptr<InputDialog> &&dialog) { dialogs.emplace_back(std::move(dialog)); }

void DialogManager::renderDialogs() {
  std::ranges::for_each(fileDialogs, &FileDialog::render);
  if (const auto iter = std::ranges::find_if(fileDialogs, [](auto &dialog) { return dialog->isDone(); });
      iter != fileDialogs.end()) {
    fileDialogBookmark = (*iter)->serializeBookmark();
    fileDialogs.erase(iter);
  }
  std::ranges::for_each(dialogs, [](auto &dialog) { dialog->render(); });
  if (const auto iter = std::ranges::find_if(dialogs, [](auto &dialog) { return dialog->isClosed(); });
      iter != dialogs.end()) {
    dialogs.erase(iter);
  }
}

void DialogManager::removeDialog(ModalDialog &dialog) {
  if (const auto iter = std::ranges::find_if(dialogs, [&dialog](const auto &ptr) { return ptr.get() == &dialog; });
      iter != dialogs.end()) {
    dialogs.erase(iter);
  }
}

}  // namespace pf::ui::ig