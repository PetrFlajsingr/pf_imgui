//
// Created by petr on 11/7/20.
//

#ifndef PF_IMGUI_ELEMENTS_FILEDIALOG_H
#define PF_IMGUI_ELEMENTS_FILEDIALOG_H

#include <include/ImGuiFileDialog/ImGuiFileDialog.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Settings for file extension filtering.
 */
struct PF_IMGUI_EXPORT FileExtensionSettings {
  explicit FileExtensionSettings(std::vector<std::string> extensions, std::string description = "",
                                 const std::optional<ImVec4> &color = std::nullopt);
  /**
   * Allowed extensions.
   */
  std::vector<std::string> extensions;
  /**
   * Description given to the user for these extensions.
   */
  std::string description;
  /**
   * Color given to these extensions.
   */
  std::optional<ImVec4> color;
};

/**
 * @brief Dialog for file selection from disk.
 *
 * This dialog is shown on top of the other windows. It should be created via ImGuiInterface::openFileDialog(...) or ImGuiInterface::openDirDialog(...).
 *
 * Dialogs are invalidated when isDone() == true.
 *
 * @warning Currently only one instance is supported. @todo: multiple instance support
 */
class PF_IMGUI_EXPORT FileDialog : public Renderable, public Labellable, public Resizable {
 public:
  /**
   * Construct FileDialog for files
   * @param elementName ID of the dialog
   * @param label Title
   * @param extSettings extension settings @see FileExtensionSettings
   * @param onSelect callback for when user selects files
   * @param onCancel callback for when user cancels selection
   * @param size size of the dialog
   * @param startPath path in which the dialog opens @todo: change to std::filesystem::path
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedFiles maximum amount of selected files
   */
  FileDialog(const std::string &elementName, const std::string &label,
             const std::vector<FileExtensionSettings> &extSettings,
             std::invocable<std::vector<std::string>> auto onSelect, std::invocable auto onCancel,
             ImVec2 size = {200, 150}, std::string startPath = ".", std::string startName = "",
             Modal modality = Modal::No, uint32_t maxSelectedFiles = 1)
      : Renderable(elementName), Labellable(label), Resizable(size), openPath(std::move(startPath)),
        defaultName(std::move(startName)), modal(modality), fileType(FileType::File), maxSelectCount(maxSelectedFiles),
        onFilesSelected(onSelect), onSelectCanceled(onCancel) {
    prepareExtInfos(extSettings);
  }

  /**
   * Construct FileDialog for directories
   * @param elementName ID of the dialog
   * @param label Title
   * @param onSelect callback for when user selects files
   * @param onCancel callback for when user cancels selection
   * @param size size of the dialog
   * @param startPath path in which the dialog opens @todo: change to std::filesystem::path
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedDirs maximum amount of selected directories
   */
  FileDialog(const std::string &elementName, const std::string &label,
             std::invocable<std::vector<std::string>> auto onSelect, std::invocable auto onCancel,
             ImVec2 size = {200, 150}, std::string startPath = ".", std::string startName = "",
             Modal modality = Modal::No, uint32_t maxSelectedDirs = 1)
      : Renderable(elementName), Labellable(label), Resizable(size), openPath(std::move(startPath)),
        defaultName(std::move(startName)), modal(modality), fileType(FileType::Directory),
        maxSelectCount(maxSelectedDirs), onFilesSelected(onSelect), onSelectCanceled(onCancel) {}

  /**
   * Check if the dialog has been closed.
   * @return true if selected or canceled, false otherwise
   */
  [[nodiscard]] bool isDone() const { return done; }

 protected:
  void renderImpl() override;

 private:
  /**
   * Transform extension settings into filter strings.
   * @param extSettings
   */
  void prepareExtInfos(const std::vector<FileExtensionSettings> &extSettings);

  /**
   * Set prepared filter strings to file dialog instance.
   */
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
