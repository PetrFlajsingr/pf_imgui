/**
 * @file DialogManager.h
 * @brief Class managing dialogs.
 * @author Petr Flajšingr
 * @date 5.5.22
 */

#ifndef IMGUI_EXPERIMENTS_DIALOGMANAGER_H
#define IMGUI_EXPERIMENTS_DIALOGMANAGER_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/FileDialog.h>
#include <pf_imgui/dialogs/InputDialog.h>
#include <pf_imgui/dialogs/MessageDialog.h>
#include <pf_imgui/dialogs/ModalDialog.h>
#include <pf_imgui/dialogs/builders/FileDialogBuilder.h>
#include <pf_imgui/dialogs/builders/InputDialogBuilder.h>
#include <pf_imgui/dialogs/builders/MessageDialogBuilder.h>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT DialogManager {
 public:
  /**
   * Create a builder for FileDialog.
   * @param type type of files to be selected
   * @return builder
   */
  [[nodiscard]] FileDialogBuilder buildFileDialog(FileDialogType type);

  /**
   * Add a separately created FileDialog, which will be destroyed upon closing.
   * @param dialog
   */
  void addFileDialog(std::unique_ptr<FileDialog> &&dialog);

  /**
   * Create a dialog. @see Dialog
   * The dialog should be built by the user.
   * @param elementName ID of the dialog
   * @param caption title
   * @return reference to the created dialog
   */
  ModalDialog &createDialog(const std::string &elementName, const std::string &caption);

  /**
   * Create a builder for MessageDialog.
   * @return builder
   */
  [[nodiscard]] MessageDialogBuilder buildMessageDialog();
  /**
   * Add a separately created MessageDialog, which will be destroyed upon closing.
   * @param dialog
   */
  void addMessageDialog(std::unique_ptr<MessageDialog> &&dialog);

  /**
   * Create a builder for InputDialog.
   * @return builder
   */
  [[nodiscard]] InputDialogBuilder buildInputDialog();
  /**
   * Add a separately created InputDialog, which will be destroyed upon closing.
   * @param dialog
   */
  void addInputDialog(std::unique_ptr<InputDialog> &&dialog);

 private:
  friend class ImGuiInterface;

  void renderDialogs();

  void removeDialog(ModalDialog &dialog);

  std::vector<std::unique_ptr<ModalDialog>> dialogs{};
  std::vector<std::unique_ptr<FileDialog>> fileDialogs{};

  std::optional<std::string> fileDialogBookmark = std::nullopt;

  std::size_t idCounter{};
};

}  // namespace pf::ui::ig

#endif  //IMGUI_EXPERIMENTS_DIALOGMANAGER_H
