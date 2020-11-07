//
// Created by petr on 11/7/20.
//

#ifndef PF_IMGUI_ELEMENTS_FILEDIALOG_H
#define PF_IMGUI_ELEMENTS_FILEDIALOG_H

#include "../enums.h"
#include <include/ImGuiFileDialog/ImGuiFileDialog.h>
#include <pf_imgui/elements/interface/LabeledElement.h>
#include <pf_imgui/_export.h>
#include <utility>

namespace pf::ui::ig {

struct PF_IMGUI_EXPORT FileExtensionSettings {
  explicit FileExtensionSettings(std::vector<std::string> extensions,
                                 std::string description = "",
                                 const std::optional<ImVec4> &color = std::nullopt);
  std::vector<std::string> extensions;
  std::string description;
  std::optional<ImVec4> color;
};

class PF_IMGUI_EXPORT FileDialog : public LabeledElement {
 public:
  FileDialog(const std::string &elementName, const std::string &caption,
             const std::vector<FileExtensionSettings> &extSettings,
             std::string startPath = ".", std::string startName = "",
             Modal modality = Modal::No, uint32_t maxSelectedFiles = 1);

  FileDialog(const std::string &elementName, const std::string &caption,
             std::string startPath = ".", std::string startName = "",
             Modal modality = Modal::No, uint32_t maxSelectedDirs = 1);

 protected:
  void renderImpl() override;

 private:
  void prepareExtInfos(const std::vector<FileExtensionSettings> &extSettings);

  void setExtInfos();

  std::string filters;
  std::vector<std::pair<std::string, ImVec4>> extColors;

  std::string openPath;
  std::string defaultName;
  Modal modal;
  FileType fileType;
  uint32_t maxSelectCount;

};

}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_FILEDIALOG_H
