/**
 * @file MessageDialog.h
 * @brief Simplified Dialog for informing a user and getting his response.
 * @author Petr Flajšingr
 * @date 17.4.21
 */

#ifndef PF_IMGUI_DIALOGS_MESSAGEDIALOG_H
#define PF_IMGUI_DIALOGS_MESSAGEDIALOG_H

#include <pf_common/enums.h>
#include <pf_imgui/dialogs/ModalDialog.h>
#include <pf_imgui/elements/Button.h>
#include <pf_imgui/elements/Text.h>
#include <pf_imgui/layouts/HorizontalLayout.h>
#include <string>

namespace pf::ui::ig {

/**
 * Default button type for MessageDialog.
 */
enum class MessageButtons { Ok = (1 << 0), Yes = (1 << 1), No = (1 << 2), Cancel = (1 << 3) };
static_assert(IsPureFlags<MessageButtons>());

/**
 * @brief Simplified Dialog for showing messages.
 * Button labels are the same as enums being used. User must close the dialog on his own in onDialogDone callbacks.
 * @tparam ButtonTypes values allowed for buttons
 *
 * @todo: type of message - info, warning, error, debug with some sort of graphic
 */
class PF_IMGUI_EXPORT MessageDialog : public ModalDialog {
 public:
  /**
   * Construct MessageDialog.
   * @param elementName ID of the dialog
   * @param title text rendered as a title
   * @param message message to show to the user
   * @param buttons allowed buttons, ButtonTypes must be bit masked
   * @param onDialogDone callback for buttons being pressed, If returns true then the dialog is closed
   * @param modal dialog modality
   */
  MessageDialog(const std::string &elementName, const std::string &title, Size size, const std::string &message,
                const Flags<MessageButtons> &buttons, std::invocable<MessageButtons> auto &&onDialogDone)
    requires(std::is_invocable_r_v<bool, decltype(onDialogDone), MessageButtons>)
  : MessageDialog(elementName, title, size, message, buttons) {
    dialogDone = std::forward<decltype(onDialogDone)>(onDialogDone);
  }

 private:
  MessageDialog(const std::string &elementName, const std::string &title, Size size, const std::string &message,
                const Flags<MessageButtons> &buttons);

  std::function<bool(MessageButtons)> dialogDone;
};
}  // namespace pf::ui::ig
#endif  // PF_IMGUI_DIALOGS_MESSAGEDIALOG_H
