//
// Created by petr on 11/7/20.
//

#include "FileDialog.h"

#include <iostream>
#include <utility>

namespace pf::ui::ig {

FileDialog::FileDialog(const std::string &elementName, const std::string &caption,
                       const std::vector<FileExtensionSettings> &extSettings,
                       std::string startPath, std::string startName, Modal modality,
                       uint32_t maxSelectedFiles)
    : Element(elementName), LabeledElement(elementName, caption),
      openPath(std::move(startPath)), defaultName(std::move(startName)),
      modal(modality), fileType(FileType::File), maxSelectCount(maxSelectedFiles) {
  prepareExtInfos(extSettings);
}

FileDialog::FileDialog(const std::string &elementName, const std::string &caption,
                       std::string startPath, std::string startName, Modal modality,
                       uint32_t maxSelectedDirs)
    : Element(elementName), LabeledElement(elementName, caption), openPath(std::move(startPath)),
      defaultName(std::move(startName)), modal(modality), fileType(FileType::Directory),
      maxSelectCount(maxSelectedDirs) {
}

FileExtensionSettings::FileExtensionSettings(std::vector<std::string> extensions, std::string description, const std::optional<ImVec4> &color)
    : extensions(std::move(extensions)), description(std::move(description)), color(color) {}
void FileDialog::prepareExtInfos(const std::vector<FileExtensionSettings> &extSettings) {
  for (const auto &[names, desc, color] : extSettings) {
    if (!desc.empty()) {
      filters += desc;
      filters += '{';
      for (const auto &name : names) {
        filters += name + ',';
        if (color.has_value()) {
          extColors.emplace_back(name, *color);
        }
      }
      filters = filters.substr(0, filters.size() - 1);
      filters += "},";
    } else {
      for (const auto &name : names) {
        filters += name + ',';
        if (color.has_value()) {
          extColors.emplace_back(name, *color);
        }
      }
      filters = filters.substr(0, filters.size() - 1);
    }
  }
  if (!filters.empty()) {
    filters = filters.substr(0, filters.size() - 1);
  }
}
void FileDialog::renderImpl() {
  const auto extCstr = fileType == FileType::File ? filters.c_str() : nullptr;
  switch (modal) {
    case Modal::Yes:
      igfd::ImGuiFileDialog::Instance()->OpenModal(getName(),
                                                    getLabel().c_str(),
                                                    extCstr,
                                                    defaultName,
                                                    maxSelectCount);
      break;
    case Modal::No:
      igfd::ImGuiFileDialog::Instance()->OpenDialog(getName(),
                                                   getLabel().c_str(),
                                                   extCstr,
                                                   defaultName,
                                                   maxSelectCount);
      break;
  }
  setExtInfos();
  if (igfd::ImGuiFileDialog::Instance()->FileDialog(getName())) {
    if (igfd::ImGuiFileDialog::Instance()->IsOk) {
      const auto filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
      const auto filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
      const auto selection = igfd::ImGuiFileDialog::Instance()->GetSelection();

      std::cout << "filePathName " << filePathName << std::endl;
          std::cout << "filePath " << filePath << std::endl;
      std::cout << "selection " << std::endl;
      for (auto &[a, b] : selection) {
        std::cout << a << "___" << b << std::endl;
      }
    }
    igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
  }
}
void FileDialog::setExtInfos() {
  for (const auto &[ext, color] : extColors) {
    igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(ext, color);
  }
}
}// namespace pf::ui::ig