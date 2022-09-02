/**
 * @file CommandPalette.h
 * @brief An element allowing for command selection.
 * @author Petr Flajšingr
 * @date 8.2.22
 */

#ifndef PF_IMGUI_ELEMENTS_COMMANDPALETTE_H
#define PF_IMGUI_ELEMENTS_COMMANDPALETTE_H

#include <imcmd_command_palette.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

// TODO: styles
/**
 * @brief A command palette like in sublime text or VSC
 */
class CommandPalette : public ElementWithID {
 public:
  /**
   * @brief Struct for construction of CommandPalette.
   */
  struct Config {
    using Parent = CommandPalette;
    Explicit<std::string_view> name; /*!< Unique name of the element */
  };
  /**
   * Construct CommandPalette
   * @param config construction args @see CommandPalette::Config
   */
  explicit CommandPalette(Config &&config);
  /**
   * Construct CommandPalette
   * @param elementName unique name of the element
   */
  explicit CommandPalette(const std::string &elementName);
  ~CommandPalette() override;

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

  /**
   * Add a listener which gets triggered when user selects a command.
   * @param listener callback
   * @return Subscription allowing for listener cancellation
   */
  Subscription addInteractListener(std::invocable auto &&listener) {
    return observableInteract.addListener(std::forward<decltype(listener)>(listener));
  }

  Font font = Font::Default();

 protected:
  void renderImpl() override;

 private:
  ImCmd::Context *context = nullptr;
  Observable_impl<> observableInteract{};
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_COMMANDPALETTE_H
