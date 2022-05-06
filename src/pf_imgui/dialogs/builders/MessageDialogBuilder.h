/**
 * @file MessageDialogBuilder.h
 * @brief Builder for MessageDialog.
 * @author Petr Flajšingr
 * @date 6.5.22
 */

#ifndef IMGUI_EXPERIMENTS_MESSAGEDIALOGBUILDER_H
#define IMGUI_EXPERIMENTS_MESSAGEDIALOGBUILDER_H

#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/MessageDialog.h>

namespace pf::ui::ig {

/**
 * @brief Convenience builder for MessageDialog.
 */
class PF_IMGUI_EXPORT MessageDialogBuilder {
 public:
  /**
   * Construct MessageDialogBuilder
   * @param parent owner of the FileDialog to be created
   */
  explicit MessageDialogBuilder(class DialogManager *parent);
  /**
   * Dialog's title.
   */
  MessageDialogBuilder &title(std::string dialogTitle);
  /**
   * Dialog's size.
   */
  MessageDialogBuilder &size(Size dialogSize);
  /**
   * Message to show to the user.
   */
  MessageDialogBuilder &message(std::string dialogMessage);
  /**
   * Allowed buttons. By default 'Ok' is shown.
   */
  MessageDialogBuilder &buttons(std::same_as<MessageButtons> auto... dialogButtons) {
    buttons_ = (Flags{dialogButtons} | ...);
    return *this;
  }
  /**
   * Callback triggered when user clicks any button.
   */
  MessageDialogBuilder &onDone(std::invocable<MessageButtons> auto &&callable)
    requires(std::is_invocable_r_v<bool, decltype(callable), MessageButtons>)
  {
    onDone_ = std::forward<decltype(callable)>(callable);
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
  Flags<MessageButtons> buttons_ = MessageButtons::Ok;
  std::function<bool(MessageButtons)> onDone_ = [](auto) { return true; };

  static inline std::uint32_t IdCounter = 0;
};

}  // namespace pf::ui::ig
#endif  //IMGUI_EXPERIMENTS_MESSAGEDIALOGBUILDER_H
