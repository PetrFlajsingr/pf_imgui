//
// Created by xflajs00 on 06.05.2022.
//

#include "FileDialogBuilder.h"
#include <pf_imgui/managers/DialogManager.h>

namespace pf::ui::ig {

FileDialogBuilder::FileDialogBuilder(DialogManager *parent, FileDialogType dialogType) : dialogManager(parent), type(dialogType) {}

FileDialogBuilder &FileDialogBuilder::label(std::string dialogLabel) {
  label_ = std::move(dialogLabel);
  return *this;
}

FileDialogBuilder &FileDialogBuilder::extension(FileExtensionSettings extensionSettings) {
  extensionSettings_.push_back(std::move(extensionSettings));
  return *this;
}

FileDialogBuilder &FileDialogBuilder::size(Size dialogSize) {
  size_ = dialogSize;
  return *this;
}

FileDialogBuilder &FileDialogBuilder::startPath(std::filesystem::path path) {
  startPath_ = std::move(path);
  return *this;
}

FileDialogBuilder &FileDialogBuilder::defaultFilename(std::string filename) {
  defaultFilename_ = std::move(filename);
  return *this;
}

FileDialogBuilder &FileDialogBuilder::maxFilesSelected(std::uint32_t max) {
  maxFilesSelected_ = max;
  return *this;
}

FileDialogBuilder &FileDialogBuilder::modal() {
  modal_ = Modal::Yes;
  return *this;
}

void FileDialogBuilder::build() {
  using namespace std::string_literals;
  switch (type) {
    case FileDialogType::Dir:
      dialogManager->addFileDialog(std::make_unique<FileDialog>("FileDialog_"s + std::to_string(IdCounter++), label_, onSelect_, onCancel_,
                                                                size_, startPath_, defaultFilename_, modal_, maxFilesSelected_));
      break;
    case FileDialogType::File:
      dialogManager->addFileDialog(std::make_unique<FileDialog>("FileDialog"s + std::to_string(IdCounter++), label_, extensionSettings_,
                                                                onSelect_, onCancel_, size_, startPath_, defaultFilename_, modal_,
                                                                maxFilesSelected_));
      break;
  }
}

}  // namespace pf::ui::ig