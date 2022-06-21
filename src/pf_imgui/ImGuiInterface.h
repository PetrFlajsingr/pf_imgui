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
#include <pf_imgui/DockBuilder.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/BackgroundDockingArea.h>
#include <pf_imgui/dialogs/CommandPaletteWindow.h>
#include <pf_imgui/dialogs/FileDialog.h>
#include <pf_imgui/dialogs/InputDialog.h>
#include <pf_imgui/dialogs/MessageDialog.h>
#include <pf_imgui/dialogs/ModalDialog.h>
#include <pf_imgui/dialogs/Window.h>
#include <pf_imgui/elements/MenuBars.h>
#include <pf_imgui/elements/OverlayGizmo.h>
#include <pf_imgui/elements/RadioGroup.h>
#include <pf_imgui/elements/StatusBar.h>
#include <pf_imgui/fwd.h>
#include <pf_imgui/icons.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/managers/DialogManager.h>
#include <pf_imgui/managers/FontManager.h>
#include <pf_imgui/managers/NotificationManager.h>
#include <string>
#include <toml++/toml.h>
#include <utility>
#include <vector>

namespace pf::ui::ig {

enum class ImGuiConfigFlags {
  NavEnableKeyboard = ImGuiConfigFlags_NavEnableKeyboard,
  NavEnableGamepad = ImGuiConfigFlags_NavEnableGamepad,
  NavEnableSetMousePos = ImGuiConfigFlags_NavEnableSetMousePos,
  NavNoCaptureKeyboard = ImGuiConfigFlags_NavNoCaptureKeyboard,
  NoMouse = ImGuiConfigFlags_NoMouse,
  NoMouseCursorChange = ImGuiConfigFlags_NoMouseCursorChange,
  DockingEnable = ImGuiConfigFlags_DockingEnable,
  ViewportsEnable = ImGuiConfigFlags_ViewportsEnable,
  // TODO: not implemented DpiEnableScaleViewports = ImGuiConfigFlags_DpiEnableScaleViewports,
  // TODO: not implemented DpiEnableScaleFonts = ImGuiConfigFlags_DpiEnableScaleFonts,
  IsSRGB = ImGuiConfigFlags_IsSRGB,
  IsTouchScreen = ImGuiConfigFlags_IsTouchScreen
};

struct ImGuiConfig {
  Flags<ImGuiConfigFlags> flags;
  toml::table config;
};

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
 * @todo: check if context is actually properly set where needed
 */
class PF_IMGUI_EXPORT ImGuiInterface : public Renderable {
 public:
  /**
   * Construct ImGuiInterface with given flags.
   * @param flags
   * @param tomlConfig config containing data of Savable elements
   */
  explicit ImGuiInterface(ImGuiConfig config);

  ~ImGuiInterface() override;

  /**
   * Get ImGuiIO from ImGui::.
   * @return ImGuiIO
   */
  [[nodiscard]] ImGuiIO &getIo() const;

  /**
   * Process platform input.
   */
  virtual void processInput() = 0;
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
   * @param window window to remove
   */
  void removeWindow(const Window &window);

  /**
   * Create a CommandPaletteWindow
   * @param windowName unique name of the window
   * @return reference to the created window
   */
  CommandPaletteWindow &createCommandPalette(const std::string &windowName);
  /**
   * Remove palette window from UI. This invalidates references to the destroyed window.
   * @param windowName unique name of the window to remove
   */
  void removePaletteWindow(const std::string &windowName);
  /**
   * Remove palette window from UI. This invalidates any references to it.
   * @param window window to remove
   */
  void removePaletteWindow(const CommandPaletteWindow &window);

  /**
   * Create a DockBuilder to layout DockSpace. This will be run at the end of the next frame.
   * @warning If the DockSpace or any of the Windows you use in it get destroyed the app will most likely crash (or UB)
   * @param dockSpace DockSpace to modify
   * @return DockBuilder
   */
  [[nodiscard]] DockBuilder &createDockBuilder(DockSpace &dockSpace);

  /**
   * Find Window by its ID.
   * @param windowName ID of the window
   * @return reference to the searched for Window or nullopt if no such window exists
   */
  std::optional<std::reference_wrapper<Window>> windowByName(const std::string &windowName);
  /**
   * Find Window by its ID.
   * @param windowName ID of the window
   * @return reference to the searched for Window or nullopt if no such window exists
   */
  [[nodiscard]] std::optional<std::reference_wrapper<const Window>> windowByName(const std::string &windowName) const;

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
  [[nodiscard]] AppMenuBar &createOrGetMenuBar();
  /**
   * Check if menu bar is present.
   * @return true if menu bar is present, false otherwise
   */
  [[nodiscard]] bool hasMenuBar() const;
  /**
   * Remove menu bar.
   */
  void removeMenuBar();

  /**
   * Create a status bar or replace the existing one.
   * @return reference to the created status bar
   */
  [[nodiscard]] AppStatusBar &createOrGetStatusBar();
  /**
   * @return true if the interface has an active status bar
   */
  [[nodiscard]] bool hasStatusBar() const;
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
   * Create a group for drag and drop elements.
   * @return newly created group
   */
  [[nodiscard]] DragNDropGroup &createDragNDropGroup();

  /**
   * Create a group for RadioButtons.
   * @return newly created group
   */
  [[nodiscard]] RadioGroup &createRadioGroup(const std::string &groupName, Persistent persistent = Persistent::No);

  [[nodiscard]] BackgroundDockingArea &createOrGetBackgroundDockingArea();

  void removeBackgroundDockingArea();
  // TODO: change this to something not insane
  template<typename... Args>
    requires(std::constructible_from<ViewportOverlayGizmo, Args...>) [
        [nodiscard]] ViewportOverlayGizmo &createOrGetViewportGizmo(Args &&...args) {
    if (viewportGizmo == nullptr) {
      viewportGizmo = std::make_unique<ViewportOverlayGizmo>(std::forward<Args>(args)...);
    }
    return *viewportGizmo;
  }

  void removeViewportGizmo();

  /**
   * Update fonts atlas using your backend.
   */
  virtual void updateFonts() = 0;

  [[nodiscard]] FontManager &getFontManager();
  [[nodiscard]] const FontManager &getFontManager() const;

  [[nodiscard]] NotificationManager &getNotificationManager();
  [[nodiscard]] const NotificationManager &getNotificationManager() const;

  [[nodiscard]] DialogManager &getDialogManager();
  [[nodiscard]] const DialogManager &getDialogManager() const;

  void render() override;

  FullColorPalette color;
  FullStyleOptions style;
  Font font = Font::Default();

 protected:
  std::unique_ptr<AppMenuBar> menuBar = nullptr;

  virtual void newFrame_impl() = 0;

  virtual void renderDrawData_impl(ImDrawData *drawData) = 0;

  virtual void updateMultiViewport();

  /**
   * @attention Override this function to provide rendering capabilities.
   */
  void renderImpl() override;

  // @warning you need to update font atlas manually when this value is set
  bool shouldUpdateFontAtlas = false;

  void setContext() const;

 private:
  friend class FontManager;

  ImGuiContext *imguiContext = nullptr;
  struct ImPlotContext *imPlotContext = nullptr;
  ImGuiIO &io;
  FontManager fontManager;
  NotificationManager notificationManager;
  DialogManager dialogManager;
  std::size_t idCounter{};

  std::vector<std::unique_ptr<Window>> windows{};
  std::unique_ptr<AppStatusBar> statusBar = nullptr;

  toml::table config;

  std::vector<std::unique_ptr<DragNDropGroup>> dragNDropGroups{};
  std::vector<std::unique_ptr<RadioGroup>> radioGroups{};

  std::vector<std::unique_ptr<CommandPaletteWindow>> commandPalettes{};

  std::vector<std::unique_ptr<DockBuilder>> dockBuilders{};
  std::unique_ptr<BackgroundDockingArea> backgroundDockingArea = nullptr;

  std::unique_ptr<ViewportOverlayGizmo> viewportGizmo{};
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_IMGUIINTERFACE_H
