/**
 * @file FileDialog.h
 * @brief Dialog for selecting files on disk.
 * @author Petr Flajšingr
 * @date 7.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_FILEDIALOG_H
#define PF_IMGUI_ELEMENTS_FILEDIALOG_H

#include <ImGuiFileDialog.h>
#include <filesystem>
#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/Resizable.h>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Settings for file extension filtering.
 */
struct PF_IMGUI_EXPORT FileExtensionSettings {
  /**
   * Allowed extensions.
   */
  std::vector<std::filesystem::path> extensions;
  /**
   * Description given to the user for these extensions.
   */
  std::string description{};
  /**
   * Color given to these extensions.
   */
  std::optional<Color> color = std::nullopt;
};

enum class FileDialogType { Dir, File };

/**
 * @brief Dialog for file selection from disk.
 *
 * This dialog is shown on top of the other windows. It should be created via ImGuiInterface::buildFileDialog(...).
 *
 * Dialogs are invalidated when isDone() == true.
 *
 * @todo: more SetFileStyle stuff
 * @todo: size constraints
 * @todo: key navigation
 * @todo: ImGuiFileDialogFlags_ConfirmOverwrite
 * @todo: thumbnails
 * @todo: custom pane
 */
class PF_IMGUI_EXPORT FileDialog : public Renderable,
                                   public Labellable,
                                   public Resizable,
                                   public AllColorCustomizable,
                                   public AllStyleCustomizable {
 public:
  /**
   * Construct FileDialog for files
   * @param elementName ID of the dialog
   * @param label Title
   * @param extSettings extension settings @see FileExtensionSettings
   * @param onSelect callback for when user selects files
   * @param onCancel callback for when user cancels selection
   * @param size size of the dialog
   * @param startPath path in which the dialog opens
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedFiles maximum amount of selected files
   */
  FileDialog(const std::string &elementName, const std::string &label,
             const std::vector<FileExtensionSettings> &extSettings,
             std::invocable<std::vector<std::filesystem::path>> auto &&onSelect, std::invocable auto &&onCancel,
             Size size = {500, 400}, std::filesystem::path startPath = ".", std::string startName = "",
             Modal modality = Modal::No, uint32_t maxSelectedFiles = 1)
      : Renderable(elementName), Labellable(label), Resizable(size), openPath(std::move(startPath)),
        defaultName(std::move(startName)), modal(modality), fileType(FileType::File), maxSelectCount(maxSelectedFiles),
        onFilesSelected(std::forward<decltype(onSelect)>(onSelect)),
        onSelectCanceled(std::forward<decltype(onCancel)>(onCancel)) {
    prepareExtInfos(extSettings);
  }

  /**
   * Construct FileDialog for directories
   * @param elementName ID of the dialog
   * @param label Title
   * @param onSelect callback for when user selects files
   * @param onCancel callback for when user cancels selection
   * @param size size of the dialog
   * @param startPath path in which the dialog opens
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedDirs maximum amount of selected directories
   */
  FileDialog(const std::string &elementName, const std::string &label,
             std::invocable<std::vector<std::filesystem::path>> auto onSelect, std::invocable auto onCancel,
             Size size = {200, 150}, std::filesystem::path startPath = ".", std::string startName = "",
             Modal modality = Modal::No, uint32_t maxSelectedDirs = 1)
      : Renderable(elementName), Labellable(label), Resizable(size), openPath(std::move(startPath)),
        defaultName(std::move(startName)), modal(modality), fileType(FileType::Directory),
        maxSelectCount(maxSelectedDirs), onFilesSelected(onSelect), onSelectCanceled(onCancel) {}

  /**
   * Check if the dialog has been closed.
   * @return true if selected or canceled, false otherwise
   */
  [[nodiscard]] bool isDone() const;

#ifdef USE_BOOKMARK
  [[nodiscard]] std::string serializeBookmark();

  void deserializeBookmark(const std::string &bookmarkStr);
#endif

 protected:
  void renderImpl() override;

 private:
  /**
   * Transform extension settings into filter strings.
   * @param extSettings
   */
  void prepareExtInfos(const std::vector<FileExtensionSettings> &extSettings);

  std::string filters;
  std::vector<std::pair<std::string, Color>> extColors;

  std::filesystem::path openPath;
  std::string defaultName;
  Modal modal;
  FileType fileType;
  uint32_t maxSelectCount;

  std::function<void(std::vector<std::filesystem::path>)> onFilesSelected;
  std::function<void()> onSelectCanceled;

  ImGuiFileDialog fileDialogInstance;

  bool done = false;
};

/**
 * @brief Convenience builder for FileDialog.
 */
class PF_IMGUI_EXPORT FileDialogBuilder {
 public:
  /**
   * Construct FileDialogBuilder
   * @param parent owner of the FileDialog to be created
   * @param dialogType type of the dialog - directory or file
   */
  explicit FileDialogBuilder(class DialogManager *parent, FileDialogType dialogType);

  /**
   * Set dialog's label
   * @param dialogLabel
   * @return builder
   */
  FileDialogBuilder &label(std::string dialogLabel);
  /**
   * Add allowed extension for file selection
   * @param extensionSettings
   * @return builder
   */
  FileDialogBuilder &extension(FileExtensionSettings extensionSettings);
  /**
   * Set callback for user selection
   * @param callable
   * @return builder
   */
  FileDialogBuilder &onSelect(std::invocable<std::vector<std::filesystem::path>> auto &&callable) {
    onSelect_ = std::forward<decltype(callable)>(callable);
    return *this;
  }
  /**
   * Set callback for user cancellation
   * @param callable
   * @return builder
   */
  FileDialogBuilder &onCancel(std::invocable auto &&callable) {
    onCancel_ = std::forward<decltype(callable)>(callable);
    return *this;
  }
  /**
   * Set dialog size
   * @param size
   * @return builder
   */
  FileDialogBuilder &size(Size dialogSize);
  /**
   * Set starting path of the dialog
   * @param path
   * @return builder
   */
  FileDialogBuilder &startPath(std::filesystem::path path);
  /**
   * Set default name of file to select
   * @param filename
   * @return builder
   */
  FileDialogBuilder &defaultFilename(std::string filename);
  /**
   * Set maximum amount of files/dirs to be selected
   * @param max
   * @return builder
   */
  FileDialogBuilder &maxFilesSelected(std::uint32_t max);
  /**
   * Enable dialog to be modal
   * @return builder
   */
  FileDialogBuilder &modal();
  /**
   * Build the dialog and show it in the next render pass.
   */
  void build();

 private:
  class DialogManager *dialogManager;
  FileDialogType type;
  std::string label_;
  std::vector<FileExtensionSettings> extensionSettings_;
  std::function<void(std::vector<std::filesystem::path>)> onSelect_;
  std::function<void()> onCancel_ = [] {};
  Size size_ = {500, 400};
  std::filesystem::path startPath_ = ".";
  std::string defaultFilename_{};
  Modal modal_ = Modal::No;
  std::uint32_t maxFilesSelected_ = 1;

  static inline std::uint32_t IdCounter = 0;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_FILEDIALOG_H
