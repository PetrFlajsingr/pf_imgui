/**
 * @file InputDialog.h
 * @brief Convenience dialog for text input.
 * @author Petr Flajšingr
 * @date 18.4.21
 */

#ifndef PF_IMGUI_DIALOGS_INPUTDIALOG_H
#define PF_IMGUI_DIALOGS_INPUTDIALOG_H

#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/ModalDialog.h>
#include <pf_imgui/elements/Button.h>
#include <pf_imgui/elements/InputText.h>
#include <pf_imgui/elements/Text.h>
#include <pf_imgui/layouts/HorizontalLayout.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Simplified dialog for user input.
 * It should be created via ImGuiInterface.
 *
 * @todo: rework this
 */
class PF_IMGUI_EXPORT InputDialog : public ModalDialog {
 public:
  /**
   * Construct InputDialog.
   * @param elementName ID of the dialog
   * @param title title of the dialog
   * @param message message shown to a user
   * @param onInput callback for user input
   * @param onCancel callback for user cancel
   * @param modal dialog modality
   */
  InputDialog(const std::string &elementName, const std::string &title, const std::string &message,
              std::invocable<std::string_view> auto &&onInput, std::invocable auto &&onCancel)
      : ModalDialog(elementName, title), inputDone(std::forward<decltype(onInput)>(onInput)),
        cancelClicked(std::forward<decltype(onCancel)>(onCancel)) {
    createChild<Text>("text", message);
    auto &input = createChild<InputText>("input", "", "", TextInputType::MultiLine);
    auto &btnLayout = createChild<HorizontalLayout>("hor_layout", Size::Auto());
    btnLayout.createChild<Button>("button_ok", "Ok").addClickListener([this, &input] {
      inputDone(input.getValue());
      close();
    });
    btnLayout.createChild<Button>("button_cancel", "Cancel").addClickListener([this] {
      cancelClicked();
      close();
    });
  }

 private:
  std::function<void(std::string_view)> inputDone;
  std::function<void()> cancelClicked;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_DIALOGS_INPUTDIALOG_H
