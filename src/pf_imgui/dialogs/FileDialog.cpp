//
// Created by petr on 11/7/20.
//

#include "FileDialog.h"
#include <utility>

namespace pf::ui::ig {

FileExtensionSettings::FileExtensionSettings(std::vector<std::string> extensions, std::string description,
                                             const std::optional<ImVec4> &color)
    : extensions(std::move(extensions)), description(std::move(description)), color(color) {}

void FileDialog::prepareExtInfos(const std::vector<FileExtensionSettings> &extSettings) {
  for (const auto &[names, desc, color] : extSettings) {
    if (!desc.empty()) {
      filters += desc;
      filters += '{';
      for (const auto &name : names) {
        filters += name + ',';
        if (color.has_value()) { extColors.emplace_back(name, *color); }
      }
      filters = filters.substr(0, filters.size() - 1);
      filters += "},";
    } else {
      for (const auto &name : names) {
        filters += name + ',';
        if (color.has_value()) { extColors.emplace_back(name, *color); }
      }
      filters = filters.substr(0, filters.size() - 1);
    }
  }
  if (!filters.empty()) { filters = filters.substr(0, filters.size() - 1); }
}

void FileDialog::renderImpl() {
  if (done) { return; }
  const auto extCstr = fileType == FileType::File ? filters.c_str() : nullptr;
  switch (modal) {
    case Modal::Yes:
      ImGuiFileDialog::Instance()->OpenModal(getName(), getLabel().c_str(), extCstr, defaultName, maxSelectCount);
      break;
    case Modal::No:
      ImGuiFileDialog::Instance()->OpenDialog(getName(), getLabel().c_str(), extCstr, defaultName, maxSelectCount);
      break;
  }
  setExtInfos();


  if (ImGuiFileDialog::Instance()->Display(getName(), ImGuiWindowFlags_NoCollapse, getSize())) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      const auto filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      const auto selection = ImGuiFileDialog::Instance()->GetSelection();

      if (!selection.empty()) {
        auto selectionVec = std::vector<std::string>();
        selectionVec.reserve(selection.size());
        for (auto &[_, path] : selection) { selectionVec.emplace_back(path); }
        onFilesSelected(selectionVec);
      } else {
        onFilesSelected({filePathName});
      }
    } else {
      onSelectCanceled();
    }
    ImGuiFileDialog::Instance()->Close();
  }
  done = true;
}

void FileDialog::setExtInfos() {
  for (const auto &[ext, color] : extColors) { ImGuiFileDialog::Instance()->SetExtentionInfos(ext, color); }
}

}// namespace pf::ui::ig