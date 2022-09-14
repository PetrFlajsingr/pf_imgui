/**
 * @file CommandPaletteWindow.h
 * @brief Special type of window which allows command selection.
 * @author Petr Flajšingr
 * @date 8.2.22
 */

#ifndef PF_IMGUI_COMMANDPALETTEWINDOW_H
#define PF_IMGUI_COMMANDPALETTEWINDOW_H

#include <imcmd_command_palette.h>
#include <pf_imgui/interface/Renderable.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief A command palette like in sublime text or VSC
 */
class CommandPaletteWindow : public Renderable {
 public:
  /**
   * Construct CommandPalette
   * @param elementName unique name of the window
   */
  explicit CommandPaletteWindow(const std::string &elementName);
  ~CommandPaletteWindow() override;

  /**
   * Add a command along with a function which should be called when it is requested
   * @param commandName name of the command
   * @param callback callback of the command
   */
  void addCommand(const std::string &commandName, std::invocable auto &&callback) {
    ImCmd::SetCurrentContext(context);
    auto command = ImCmd::Command{};
    command.Name = commandName;
    command.InitialCallback = std::forward<decltype(callback)>(callback);
    ImCmd::AddCommand(std::move(command));
  }
  /**
   * Remove given command from the palette
   * @param commandName name of the command to remove
   */
  void removeCommand(const std::string &commandName);

  /**
   * Move application's focus to search box of this palette.
   */
  void moveFocusToSearchBox();
  /**
   * Prepare text into palette's search field.
   * @param text search text
   */
  void setSearchText(const std::string &text);

 protected:
  void renderImpl() override;

 private:
  ImCmd::Context *context = nullptr;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_COMMANDPALETTEWINDOW_H
