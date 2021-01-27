//
// Created by petr on 11/7/20.
//

#ifndef PF_IMGUI_ELEMENTS_FILEDIALOG_H
#define PF_IMGUI_ELEMENTS_FILEDIALOG_H

#include "../enums.h"
#include <include/ImGuiFileDialog/ImGuiFileDialog.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Labellable.h>
#include <pf_imgui/elements/interface/Renderable.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

struct PF_IMGUI_EXPORT FileExtensionSettings {
  explicit FileExtensionSettings(std::vector<std::string> extensions, std::string description = "",
                                 const std::optional<ImVec4> &color = std::nullopt);
  std::vector<std::string> extensions;
  std::string description;
  std::optional<ImVec4> color;
};

class PF_IMGUI_EXPORT FileDialog : public Renderable, public Labellable {
 public:
  FileDialog(const std::string &elementName, const std::string &label,
             const std::vector<FileExtensionSettings> &extSettings,
             std::invocable<std::vector<std::string>> auto onSelect, std::invocable auto onCancel,
             std::string startPath = ".", std::string startName = "", Modal modality = Modal::No,
             uint32_t maxSelectedFiles = 1)
      : Renderable(elementName), Labellable(label), openPath(std::move(startPath)), defaultName(std::move(startName)),
        modal(modality), fileType(FileType::File), maxSelectCount(maxSelectedFiles), onFilesSelected(onSelect),
        onSelectCanceled(onCancel) {
    prepareExtInfos(extSettings);
  }

  FileDialog(const std::string &elementName, const std::string &label,
             std::invocable<std::vector<std::string>> auto onSelect, std::invocable auto onCancel,
             std::string startPath = ".", std::string startName = "", Modal modality = Modal::No,
             uint32_t maxSelectedDirs = 1)
      : Renderable(elementName), Labellable(label), openPath(std::move(startPath)), defaultName(std::move(startName)),
        modal(modality), fileType(FileType::Directory), maxSelectCount(maxSelectedDirs), onFilesSelected(onSelect),
        onSelectCanceled(onCancel) {}

  [[nodiscard]] bool isDone() const { return done; }

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

  std::function<void(std::vector<std::string>)> onFilesSelected;
  std::function<void()> onSelectCanceled;

  bool done = false;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_FILEDIALOG_H
