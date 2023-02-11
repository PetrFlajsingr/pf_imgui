/**
 * @file FileDialog.h
 * @brief Dialog for selecting files on disk.
 * @author Petr Flajšingr
 * @date 7.11.20
 */

#ifndef PF_IMGUI_DIALOGS_FILEDIALOG_H
#define PF_IMGUI_DIALOGS_FILEDIALOG_H

#include <filesystem>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Color.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/enums.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <string>
#include <utility>
#include <vector>

// fwd for ImGuiFileDialog
namespace IGFD {
class FileDialog;
}

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
class PF_IMGUI_EXPORT FileDialog : public Renderable {
 public:
  /**
   * Construct FileDialog for files
   * @param elementName ID of the dialog
   * @param labelText Title
   * @param extSettings extension settings @see FileExtensionSettings
   * @param onSelect callback for when user selects files
   * @param onCancel callback for when user cancels selection
   * @param initialSize size of the dialog
   * @param startPath path in which the dialog opens
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedFiles maximum amount of selected files
   */
  FileDialog(std::string_view elementName, std::string_view labelText, const std::vector<FileExtensionSettings> &extSettings,
             std::invocable<std::vector<std::filesystem::path>> auto &&onSelect, std::invocable auto &&onCancel,
             Size initialSize = {500, 400}, std::filesystem::path startPath = ".", std::string startName = "", Modal modality = Modal::No,
             uint32_t maxSelectedFiles = 1)
      : FileDialog(FileType::File, elementName, labelText, extSettings, std::forward<decltype(onSelect)>(onSelect),
                   std::forward<decltype(onCancel)>(onCancel), initialSize, startPath, startName, modality, maxSelectedFiles) {}

  /**
   * Construct FileDialog for directories
   * @param elementName ID of the dialog
   * @param label Title
   * @param onSelect callback for when user selects files
   * @param onCancel callback for when user cancels selection
   * @param initialSize size of the dialog
   * @param startPath path in which the dialog opens
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedDirs maximum amount of selected directories
   */
  FileDialog(std::string_view elementName, std::string_view labelText, std::invocable<std::vector<std::filesystem::path>> auto &&onSelect,
             std::invocable auto &&onCancel, Size initialSize = {200, 150}, std::filesystem::path startPath = ".",
             std::string startName = "", Modal modality = Modal::No, uint32_t maxSelectedDirs = 1)
      : FileDialog(FileType::Directory, elementName, labelText, {}, std::forward<decltype(onSelect)>(onSelect),
                   std::forward<decltype(onCancel)>(onCancel), initialSize, startPath, startName, modality, maxSelectedDirs) {}

  ~FileDialog() override;

  /**
   * Check if the dialog has been closed.
   * @return true if selected or canceled, false otherwise
   */
  [[nodiscard]] bool isDone() const;

  [[nodiscard]] std::string serializeBookmark();

  void deserializeBookmark(const std::string &bookmarkStr);

  FullColorPalette color;
  FullStyleOptions style;
  Font font = Font::Default();
  Property<Label> label;

  Property<Size> size;

 protected:
  void renderImpl() override;

 private:
  /**
   * Constructor to hide implementation.
   */
  FileDialog(FileType dialogFileType, std::string_view elementName, std::string_view labelText,
             const std::vector<FileExtensionSettings> &extSettings, std::function<void(std::vector<std::filesystem::path>)> onSelect,
             std::function<void()> onCancel, Size initialSize, std::filesystem::path startPath, std::string startName, Modal modality,
             uint32_t maxSelected);
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

  std::unique_ptr<IGFD::FileDialog> fileDialogInstance;

  bool done = false;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_DIALOGS_FILEDIALOG_H
