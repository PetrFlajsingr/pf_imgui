/**
 * @file FileDialogBuilder.h
 * @brief Builder for FileDialog.
 * @author Petr Flajšingr
 * @date 5.5.22
 */

#ifndef IMGUI_EXPERIMENTS_FILEDIALOGBUILDER_H
#define IMGUI_EXPERIMENTS_FILEDIALOGBUILDER_H

#include <concepts>
#include <filesystem>
#include <functional>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/FileDialog.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

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

#endif  //IMGUI_EXPERIMENTS_FILEDIALOGBUILDER_H
