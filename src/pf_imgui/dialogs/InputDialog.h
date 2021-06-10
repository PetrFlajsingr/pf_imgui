/**
 * @file InputDialog.h
 * @brief Convenience dialog for text input.
 * @author Petr Flajšingr
 * @date 18.4.21
 */

#ifndef PF_IMGUI_DIALOGS_INPUTDIALOG_H
#define PF_IMGUI_DIALOGS_INPUTDIALOG_H

#include <pf_imgui/dialogs/ModalDialog.h>
#include <pf_imgui/elements/Button.h>
#include <pf_imgui/elements/InputText.h>
#include <pf_imgui/elements/Text.h>
#include <pf_imgui/layouts/BoxLayout.h>

namespace pf::ui::ig {

/**
 * @brief Simplified dialog for user input.
 * It should be created via ImGuiInterface.
 */
class InputDialog : public ModalDialog {
 public:
  /**
   * Construct InputDialog.
   * @param parent owner
   * @param elementName ID of the dialog
   * @param title title of the dialog
   * @param message message shown to a user
   * @param onInput callback for user input
   * @param onCancel callback for user cancel
   * @param modal dialog modality
   */
  InputDialog(ImGuiInterface &parent, const std::string &elementName, const std::string &title,
              const std::string &message, std::invocable<std::string> auto &&onInput, std::invocable auto &&onCancel)
      : ModalDialog(parent, elementName, title), inputDone(std::forward<decltype(onInput)>(onInput)),
        cancelClicked(std::forward<decltype(onCancel)>(onCancel)) {
    createChild<Text>(getName() + "text", message);
    auto &input = createChild<InputText>(getName() + "input", "", "", TextInputType::MultiLine);
    auto &btnLayout = createChild<BoxLayout>(getName() + "box_layout", LayoutDirection::LeftToRight, Size::Auto());
    btnLayout.createChild<Button>(getName() + "_button_ok", "Ok").template addClickListener([this, &input] {
      inputDone(input.getText());
      close();
    });
    btnLayout.createChild<Button>(getName() + "_button_cancel", "Cancel").template addClickListener([this] {
      cancelClicked();
      close();
    });
  }

 private:
  std::function<void(std::string)> inputDone;
  std::function<void()> cancelClicked;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_DIALOGS_INPUTDIALOG_H
