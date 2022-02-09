//
// Created by petr on 11/7/20.
//

#include "FileDialog.h"
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {
// TODO: fix multi dir selection - when using GetSelection() it returns invalid path

bool FileDialog::isDone() const { return done; }

void FileDialog::prepareExtInfos(const std::vector<FileExtensionSettings> &extSettings) {
  for (const auto &[extNames, desc, color] : extSettings) {
    if (!desc.empty()) {
      filters += desc;
      filters += '{';
      for (const auto &extName : extNames) {
        filters += fmt::format(".{},", extName.string());
        if (color.has_value()) { extColors.emplace_back(extName.string(), *color); }
      }
      filters = filters.substr(0, filters.size() - 1);
      filters += "},";
    } else {
      for (const auto &extName : extNames) {
        filters += fmt::format("{},", extName.string());
        if (color.has_value()) { extColors.emplace_back(extName.string(), *color); }
      }
      filters = filters.substr(0, filters.size() - 1);
    }
  }
  if (!filters.empty()) { filters = filters.substr(0, filters.size() - 1); }
}

void FileDialog::renderImpl() {
  if (done) { return; }
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  switch (modal) {
    case Modal::Yes:
      fileDialogInstance.OpenModal(getName(), getLabel(), fileType == FileType::File ? filters.c_str() : nullptr,
                                   openPath.string(), defaultName, static_cast<int>(maxSelectCount));
      break;
    case Modal::No:
      fileDialogInstance.OpenDialog(getName(), getLabel(), fileType == FileType::File ? filters.c_str() : nullptr,
                                    openPath.string(), defaultName, static_cast<int>(maxSelectCount));
      break;
  }
  // TODO: more style options and icons
  std::ranges::for_each(extColors, [this] (const auto &extColor) {
    const auto &[ext, color] = extColor;
    fileDialogInstance.SetFileStyle(IGFD_FileStyleByExtention, ext.c_str(), color);
  });

  if (fileDialogInstance.Display(getName(), ImGuiWindowFlags_NoCollapse, getSize().asImVec())) {
    RAII end{[&] { fileDialogInstance.Close(); }};
    if (fileDialogInstance.IsOk()) {
      const auto filePathName = fileDialogInstance.GetFilePathName();
      const auto selection = fileDialogInstance.GetSelection();

      if (!selection.empty() && fileType != FileType::Directory) {
        auto selectionVec = std::vector<std::filesystem::path>();
        selectionVec.reserve(selection.size());
        for (auto &[_, path] : selection) { selectionVec.emplace_back(path); }
        onFilesSelected(selectionVec);
      } else {
        onFilesSelected({filePathName});
      }
    } else {
      onSelectCanceled();
    }
    done = true;
  }
}

FileDialogBuilder::FileDialogBuilder(ImGuiInterface *parent, FileDialogType dialogType)
    : imguiInterface(parent), type(dialogType) {}

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
      imguiInterface->addFileDialog(FileDialog{"FileDialog"s + std::to_string(IdCounter++), label_, onSelect_,
                                               onCancel_, size_, startPath_, defaultFilename_, modal_,
                                               maxFilesSelected_});
      break;
    case FileDialogType::File:
      imguiInterface->addFileDialog(FileDialog{"FileDialog"s + std::to_string(IdCounter++), label_, extensionSettings_,
                                               onSelect_, onCancel_, size_, startPath_, defaultFilename_, modal_,
                                               maxFilesSelected_});
      break;
  }
}

}  // namespace pf::ui::ig
