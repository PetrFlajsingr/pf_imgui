/**
 * @file ImGuiInterface.h
 * @brief Main class of the library.
 * @author Petr Flajšingr
 * @date 27.10.20
 */
#ifndef PF_IMGUI_IMGUIINTERFACE_H
#define PF_IMGUI_IMGUIINTERFACE_H

#include <imgui.h>
#include <memory>
#include <pf_common/coroutines/Sequence.h>
#include <pf_imgui/FontManager.h>
#include <pf_imgui/NotificationManager.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/FileDialog.h>
#include <pf_imgui/dialogs/InputDialog.h>
#include <pf_imgui/dialogs/MessageDialog.h>
#include <pf_imgui/dialogs/ModalDialog.h>
#include <pf_imgui/dialogs/Window.h>
#include <pf_imgui/elements/MenuBars.h>
#include <pf_imgui/elements/StatusBar.h>
#include <pf_imgui/fwd.h>
#include <pf_imgui/icons.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <string>
#include <toml++/toml.h>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Root class of an entire UI tree. Provides an ability to create windows and dialogs.
 *
 * The class has it's own menu bar, which is applied to the entire render window.
 *
 * It also takes care of config saving for Savable elements.
 *
 * @attention: For actual use it needs to be inherited from and render_impl should be reimplemented with your own rendering logic.
 * You also need to update fonts when required based on variable shouldUpdateFontAtlas.
 * @todo: localization
 * @todo: key bindings?
 * @todo: icon loading from memory not just from file - implement that in backends as well
 */
class PF_IMGUI_EXPORT ImGuiInterface : public Renderable, public AllStyleCustomizable, public AllColorCustomizable {
 public:
  /**
   * Construct ImGuiInterface with given flags.
   * @param flags
   * @param tomlConfig config containing data of Savable elements
   */
  explicit ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig, bool enableMultiViewport,
                          const std::filesystem::path &iconFontDirectory = ".",
                          const Flags<IconPack> &enabledIconPacks = Flags<IconPack>{}, float iconSize = 16.f);

  /**
   * Get ImGuiIO from ImGui::.
   * @return ImGuiIO
   */
  [[nodiscard]] ImGuiIO &getIo() const;

  /**
   * Create a dialog. @see Dialog
   * The dialog should be built by the user.
   * @param elementName ID of the dialog
   * @param caption title
   * @return reference to the created dialog
   */
  ModalDialog &createDialog(const std::string &elementName, const std::string &caption);

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
    auto dialog = std::make_unique<MessageDialog>(*this, "MsgDialog"s + std::to_string(getNext(idGen)), title, message,
                                                  buttons, std::forward<decltype(onDialogDone)>(onDialogDone));
    dialogs.emplace_back(std::move(dialog));
  }

  /**
   * Create a Window.
   * @param windowName ID of the window
   * @param title title
   * @return reference to the created window
   */
  Window &createWindow(const std::string &windowName, std::string title);
  /**
   * Remove the window from UI. This invalidates any references to the window.
   * @param windowName ID of the window to remove
   */
  void removeWindow(const std::string &windowName);
  /**
   * Remove the window from UI. This invalidates any references to the window.
   * @param name ID of the window to remove
   */
  void removeWindow(const Window &window);

  /**
   * Find Window by its ID.
   * @param windowName ID of the window
   * @throws IdNotFoundException when the Window of given ID is not present
   * @return reference to the searched for Window or nullopt if no such window exists
   */
  std::optional<std::reference_wrapper<Window>> windowByName(const std::string &windowName);

  /**
   * Get all windows present in the UI.
   * @return view to references of all windows
   */
  inline auto getWindows() {
    return windows | ranges::views::transform([](auto &window) -> Window & { return *window; });
  }
  /**
   * Get all windows present in the UI.
   * @return view to const references of all windows
   */
  [[nodiscard]] inline auto getWindows() const {
    return windows | ranges::views::transform([](const auto &window) -> const Window & { return *window; });
  }

  /**
   * Get menu bar of the application. If the menubar doesn't exist, it is created.
   * @return reference to the applications menu bar
   */
  [[nodiscard]] AppMenuBar &getMenuBar();
  /**
   * Check if menu bar is present.
   * @return true if menu bar is present, false otherwise
   */
  [[nodiscard]] bool hasMenuBar() const;

  /**
   * Create a status bar or replace the existing one.
   * @param barName unique name of the element
   * @return reference to the created status bar
   */
  [[nodiscard]] AppStatusBar &createStatusBar(const std::string &barName);

  /**
   * Remove status bar.
   */
  void removeStatusBar();

  /**
   * Get config, which contains data of Savable elements.
   * @attention Call updateConfig() in order to collect data from the UI tree.
   * @return toml data of Savable elements
   */
  [[nodiscard]] const toml::table &getConfig() const;

  /**
   * Check if the window is hovered.
   * @return true if the window is hovered, false otherwise
   */
  [[nodiscard]] bool isWindowHovered() const;
  /**
   * Check if keyboard is currently being capture by UI.
   * @return true if the keyboard is captured, false otherwise
   */
  [[nodiscard]] bool isKeyboardCaptured() const;

  /**
   * Collect data from the UI tree and save serialized values into toml table.
   * @details Use getConfig() to collect saved data.
   */
  void updateConfig();
  /**
   * Set state from inner config file.
   */
  void setStateFromConfig();

  /**
   * Set new style to the UI.
   * @param styleSetter callback modifying style of the UI
   */
  void setStyle(std::invocable<ImGuiStyle &> auto styleSetter) { styleSetter(ImGui::GetStyle()); }

  /**
   * Create and open a file selection dialog. @see FileDialog
   * @param caption Title
   * @param extSettings extension settings @see FileExtensionSettings
   * @param onSelect callback for when user selects files
   * @param onCancel callback for when user cancels selection
   * @param size size of the dialog
   * @param startPath path in which the dialog opens
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedFiles maximum amount of selected files
   */
  [[deprecated]] void openFileDialog(const std::string &caption, const std::vector<FileExtensionSettings> &extSettings,
                      std::invocable<std::vector<std::filesystem::path>> auto onSelect, std::invocable auto onCancel,
                      Size size = {200, 150}, std::filesystem::path startPath = ".", std::string startName = "",
                      Modal modality = Modal::No, uint32_t maxSelectedFiles = 1) {
    using namespace std::string_literals;
    fileDialogs.emplace_back("FileDialog"s + std::to_string(getNext(idGen)), caption, extSettings, onSelect, onCancel,
                             size, startPath, startName, modality, maxSelectedFiles);
  }

  /**
   * Create and open a directory selection dialog. @see FileDialog
   * @param caption Title
   * @param onSelect callback for when user selects files
   * @param onCancel callback for when user cancels selection
   * @param size size of the dialog
   * @param startPath path in which the dialog opens
   * @param startName default name for selected file/dir
   * @param modality modality of the dialog
   * @param maxSelectedDirs maximum amount of selected directories
   */
  [[deprecated]] void openDirDialog(const std::string &caption, std::invocable<std::vector<std::filesystem::path>> auto onSelect,
                     std::invocable auto onCancel, Size size = {200, 150}, std::filesystem::path startPath = ".",
                     std::string startName = "", Modal modality = Modal::No, uint32_t maxSelectedDirs = 1) {
    using namespace std::string_literals;
    fileDialogs.emplace_back("FileDialog"s + std::to_string(getNext(idGen)), caption, onSelect, onCancel, size,
                             startPath, startName, modality, maxSelectedDirs);
  }

  [[nodiscard]] FileDialogBuilder buildFileDialog(FileDialogType type);

  void addFileDialog(FileDialog &&dialog);

  /**
   * Create InputDialog.
   * @param title title of the dialog
   * @param message message shown to a user
   * @param onInput callback on user input
   * @param onCancel callback on dialog cancel
   */
  void openInputDialog(const std::string &title, const std::string &message, std::invocable<std::string> auto &&onInput,
                       std::invocable auto &&onCancel) {
    using namespace std::string_literals;
    auto dialog = std::make_unique<InputDialog>(*this, "InputDialog"s + std::to_string(getNext(idGen)), title, message,
                                                std::forward<decltype(onInput)>(onInput),
                                                std::forward<decltype(onCancel)>(onCancel));
    dialogs.emplace_back(std::move(dialog));
  }

  ///**
  // * Show a temporary notification with custom text.
  // * @param type type of notification - different icons
  // * @param message message to show to the user
  // * @param dismissTime after this time the notification will disappear
  // */
  //void showNotification(NotificationType type, std::string_view message,
  //                      std::chrono::milliseconds dismissTime = std::chrono::milliseconds{NOTIFY_DEFAULT_DISMISS});

  ///**
  // * Show a temporary notification with custom text.
  // * @param type type of notification - different icons
  // * @param title custom notification title
  // * @param message message to show to the user
  // * @param dismissTime after this time the notification will disappear
  // */
  //void showNotification(NotificationType type, std::string_view title, std::string_view message,
  //                      std::chrono::milliseconds dismissTime = std::chrono::milliseconds{NOTIFY_DEFAULT_DISMISS});

  /**
   * Create a group for drag and drop elements.
   * @return newly created group
   */
  [[nodiscard]] DragNDropGroup &createDragNDropGroup();

  /**
   * Update fonts atlas using your backend.
   */
  virtual void updateFonts() = 0;

  [[nodiscard]] FontManager &getFontManager();
  [[nodiscard]] NotificationManager &getNotificationManager();

 protected:
  std::unique_ptr<AppMenuBar> menuBar = nullptr;

  /**
   * Render all file dialogs which are currently open.
   */
  void renderDialogs();

  /**
   * @attention Override this function to provide rendering capabilities.
   */
  void renderImpl() override;

  // @warning you need to update font atlas manually when this value is set
  bool shouldUpdateFontAtlas = false;

 private:
  friend class ModalDialog;
  friend class FontManager;

  std::vector<std::unique_ptr<ModalDialog>> dialogs;
  static ImGuiIO &baseInit(ImGuiConfigFlags flags);
  ImGuiIO &io;
  FontManager fontManager;
  NotificationManager notificationManager;
  std::vector<FileDialog> fileDialogs;
  cppcoro::generator<std::size_t> idGen = iota<std::size_t>();

  std::vector<std::unique_ptr<Window>> windows;
  std::unique_ptr<AppStatusBar> statusBar = nullptr;

  toml::table config;

  std::vector<DragNDropGroup> dragNDropGroups;
  //std::vector<ImGuiToast> notifications;// TODO: change this so the user can create their own

  void removeDialog(ModalDialog &dialog);
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUIINTERFACE_H
