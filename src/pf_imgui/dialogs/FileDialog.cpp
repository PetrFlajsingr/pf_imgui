//
// Created by petr on 11/7/20.
//

#include "FileDialog.h"

namespace pf::ui::ig {
// TODO: fix multi dir selection - when using GetSelection() it returns invalid path
FileExtensionSettings::FileExtensionSettings(std::vector<std::filesystem::path> extensions, std::string description,
                                             const std::optional<ImVec4> &color)
    : extensions(std::move(extensions)), description(std::move(description)), color(color) {}

bool FileDialog::isDone() const { return done; }

void FileDialog::prepareExtInfos(const std::vector<FileExtensionSettings> &extSettings) {
  for (const auto &[names, desc, color] : extSettings) {
    if (!desc.empty()) {
      filters += desc;
      filters += '{';
      for (const auto &name : names) {
        filters += fmt::format(".{},", name.string());
        if (color.has_value()) { extColors.emplace_back(name.string(), *color); }
      }
      filters = filters.substr(0, filters.size() - 1);
      filters += "},";
    } else {
      for (const auto &name : names) {
        filters += fmt::format("{},", name.string());
        if (color.has_value()) { extColors.emplace_back(name.string(), *color); }
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
  const auto extCstr = fileType == FileType::File ? filters.c_str() : nullptr;
  switch (modal) {
    case Modal::Yes:
      fileDialogInstance.OpenModal(getName(), getLabel(), extCstr, openPath.string(), defaultName, static_cast<int>(maxSelectCount));
      break;
    case Modal::No:
      fileDialogInstance.OpenDialog(getName(), getLabel(), extCstr, openPath.string(), defaultName, static_cast<int>(maxSelectCount));
      break;
  }
  setExtInfos();

  if (fileDialogInstance.Display(getName(), ImGuiWindowFlags_NoCollapse, getSize().asImVec())) {
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
    fileDialogInstance.Close();
    done = true;
  }
}
void FileDialog::setExtInfos() {
  for (const auto &[ext, color] : extColors) { fileDialogInstance.SetExtentionInfos(fmt::format(".{}", ext), color); }
}

}// namespace pf::ui::ig