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
#include <pf_imgui/Color.h>
#include <pf_imgui/Font.h>
#include <pf_imgui/Label.h>
#include <pf_imgui/Size.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/reactive/Observable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
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
  FileDialog(const std::string &elementName, const std::string &labelText,
             const std::vector<FileExtensionSettings> &extSettings,
             std::invocable<std::vector<std::filesystem::path>> auto &&onSelect, std::invocable auto &&onCancel,
             Size initialSize = {500, 400}, std::filesystem::path startPath = ".", std::string startName = "",
             Modal modality = Modal::No, uint32_t maxSelectedFiles = 1)
      : Renderable(elementName), size(initialSize), label(labelText), openPath(std::move(startPath)),
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
   * @param initialSize size of the dialog
   * @param startPath path in which the dialog opens
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedDirs maximum amount of selected directories
   */
  FileDialog(const std::string &elementName, const std::string &labelText,
             std::invocable<std::vector<std::filesystem::path>> auto onSelect, std::invocable auto onCancel,
             Size initialSize = {200, 150}, std::filesystem::path startPath = ".", std::string startName = "",
             Modal modality = Modal::No, uint32_t maxSelectedDirs = 1)
      : Renderable(elementName), size(initialSize), label(labelText), openPath(std::move(startPath)),
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

  FullColorPalette color;
  FullStyleOptions style;
  Font font = Font::Default();
  Observable<Label> label;

  Observable<Size> size;

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

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_FILEDIALOG_H
