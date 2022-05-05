//
// Created by xflajs00 on 05.05.2022.
//

#ifndef IMGUI_EXPERIMENTS_DIALOGMANAGER_H
#define IMGUI_EXPERIMENTS_DIALOGMANAGER_H

#include <pf_imgui/dialogs/MessageDialog.h>
#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/FileDialog.h>
#include <pf_imgui/dialogs/InputDialog.h>
#include <pf_imgui/dialogs/ModalDialog.h>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT DialogManager {
 public:
  /**
   * Create a builder for FileDialog.
   * @param type type of files to be selected
   * @return builder
   */
  [[nodiscard]] FileDialogBuilder buildFileDialog(FileDialogType type) { return FileDialogBuilder(this, type); }

  /**
   * Add a separately created FileDialog, which will be destroyed upon closing.
   * @param dialog
   */
  void addFileDialog(std::unique_ptr<FileDialog> &&dialog) {
    auto &dialogRef = fileDialogs.emplace_back(std::move(dialog));
    if (fileDialogBookmark.has_value()) { dialogRef->deserializeBookmark(*fileDialogBookmark); }
  }

  /**
   * Create a dialog. @see Dialog
   * The dialog should be built by the user.
   * @param elementName ID of the dialog
   * @param caption title
   * @return reference to the created dialog
   */
  ModalDialog &createDialog(const std::string &elementName, const std::string &caption) {
    auto dialog = std::make_unique<ModalDialog>(elementName, caption);
    const auto ptr = dialog.get();
    dialogs.emplace_back(std::move(dialog));
    return *ptr;
  }

  /**
   * Create MessageDialog
   * @tparam ButtonTypes enum type used to represent buttons
   * @param title title of the dialog
   * @param message message shown to a user
   * @param buttons allowed buttons
   * @param onDialogDone callback for user interaction
   */
  template<typename ButtonTypes = MessageButtons>
  void createMsgDlg(const std::string &title, const std::string &message, Flags<ButtonTypes> buttons,
                    std::invocable<ButtonTypes> auto &&onDialogDone) {
    using namespace std::string_literals;
    auto dialog = std::make_unique<MessageDialog>(*this, "MsgDialog"s + std::to_string(idCounter++), title, message,
                                                  buttons, std::forward<decltype(onDialogDone)>(onDialogDone));
    dialogs.emplace_back(std::move(dialog));
  }


  /**
   * Create InputDialog.
   * @param title title of the dialog
   * @param message message shown to a user
   * @param onInput callback on user input
   * @param onCancel callback on dialog cancel
   * @todo: use builder instead?
   */
  void openInputDialog(const std::string &title, const std::string &message, std::invocable<std::string> auto &&onInput,
                       std::invocable auto &&onCancel) {
    using namespace std::string_literals;
    auto dialog = std::make_unique<InputDialog>(*this, "InputDialog"s + std::to_string(idCounter++), title, message,
                                                std::forward<decltype(onInput)>(onInput),
                                                std::forward<decltype(onCancel)>(onCancel));
    dialogs.emplace_back(std::move(dialog));
  }

 private:
  friend class ImGuiInterface;

  void renderDialogs() {
    std::ranges::for_each(fileDialogs, &FileDialog::render);
    if (const auto iter = std::ranges::find_if(fileDialogs, [](auto &dialog) { return dialog->isDone(); });
        iter != fileDialogs.end()) {
      fileDialogBookmark = (*iter)->serializeBookmark();
      fileDialogs.erase(iter);
    }
    std::ranges::for_each(dialogs, [](auto &dialog) { dialog->render(); });
    if (const auto iter = std::ranges::find_if(dialogs, [](auto &dialog) { return dialog->isClosed(); });
        iter != dialogs.end()) {
      dialogs.erase(iter);
    }
  }

  void removeDialog(ModalDialog &dialog) {
    if (const auto iter = std::ranges::find_if(dialogs, [&dialog](const auto &ptr) { return ptr.get() == &dialog; });
        iter != dialogs.end()) {
      dialogs.erase(iter);
    }
  }

  std::vector<std::unique_ptr<ModalDialog>> dialogs{};
  std::vector<std::unique_ptr<FileDialog>> fileDialogs{};

  std::optional<std::string> fileDialogBookmark = std::nullopt;

  std::size_t idCounter{};
};

}  // namespace pf::ui::ig

#endif  //IMGUI_EXPERIMENTS_DIALOGMANAGER_H
