/**
 * @file InputDialogBuilder.h
 * @brief Builder for InputDIalog.
 * @author Petr Flajšingr
 * @date 6.5.22
 */

#ifndef PF_IMGUI_DIALOGS_BUILDERS_INPUTDIALOGBUILDER_H
#define PF_IMGUI_DIALOGS_BUILDERS_INPUTDIALOGBUILDER_H

#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/InputDialog.h>

namespace pf::ui::ig {

/**
 * @brief Convenience builder for InputDialog.
 */
class PF_IMGUI_EXPORT InputDialogBuilder {
 public:
  /**
   * Construct InputDialogBuilder
   * @param parent owner of the FileDialog to be created
   */
  explicit InputDialogBuilder(class DialogManager *parent);
  /**
   * Dialog's title.
   */
  InputDialogBuilder &title(std::string dialogTitle);
  /**
   * Dialog's size.
   */
  InputDialogBuilder &size(Size dialogSize);
  /**
   * Message to show to the user.
   */
  InputDialogBuilder &message(std::string dialogMessage);
  /**
   * Callback triggered when user sends an input.
   */
  InputDialogBuilder &onInput(std::invocable<std::string_view> auto &&callable) {
    onInput_ = std::forward<decltype(callable)>(callable);
    return *this;
  }
  /**
   * Callback triggered when user cancels the dialog.
   */
  InputDialogBuilder &onCancel(std::invocable auto &&callable) {
    onCancel_ = std::forward<decltype(callable)>(callable);
    return *this;
  }
  /**
   * Create new dialog and place it inside DialogManager.
   */
  void build();

 private:
  class DialogManager *dialogManager;
  std::string title_;
  Size size_{400, 200};
  std::string message_;
  std::function<void(std::string_view)> onInput_ = [](auto) {};
  std::function<void()> onCancel_ = []() {};

  static inline std::uint32_t IdCounter = 0;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_DIALOGS_BUILDERS_INPUTDIALOGBUILDER_H
