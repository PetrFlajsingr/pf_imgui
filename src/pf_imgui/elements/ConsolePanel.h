/**
 * @file ConsolePanel.h
 * @brief A simple console element.
 * @author Petr Flajšingr
 * @date 23.6.22
 */

#ifndef PF_IMGUI_ELEMENTS_CONSOLEPANEL_H
#define PF_IMGUI_ELEMENTS_CONSOLEPANEL_H

#include <pf_imgui/Color.h>
#include <pf_imgui/details/CustomIconButtonImpls.h>
#include <pf_imgui/elements/details/TextUtils.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>

namespace pf::ui::ig {

/**
 * @brief A console element with command history and basic command completion.
 */
class ConsolePanel : public ElementWithID, public Resizable, public Savable {
  enum class RecordType { Input, Output };

 public:
  /**
   * @brief ConsolePanel's record.
   */
  struct Record {
    std::string message;
    std::optional<Color> color;
    std::optional<Color> backgroundColor;
  };

  /**
   * Construction args for ConsolePanel
   */
  struct Config {
    using Parent = ConsolePanel;
    Explicit<std::string> name; /*!< Unique name of the element */
    Size size = Size::Auto();   /*!< Size of the element */
    bool persistent = false;    /*!< Enable disk state saving */
  };
  /**
   * Construct ConsolePanel with Config @see ConsolePanel::Config
   * @param config construction args
   */
  ConsolePanel(Config &&config);
  /**
   * Construct ConsolePanel
   * @param elementName unique name of the element
   * @param initialSize size of the element
   * @param persistent enable disk state saving
   */
  ConsolePanel(const std::string &elementName, Size initialSize, Persistent persistent = Persistent::No);

  /**
   * Set handler for console inputs. Returned record is rendered after the request.
   */
  void setInputHandler(std::invocable<std::string_view> auto &&fnc)
    requires(std::same_as<std::invoke_result_t<decltype(fnc), std::string_view>, std::optional<Record>>)
  {
    inputHandler = std::forward<decltype(fnc)>(fnc);
  }

  /**
   * Output rendered in text window as a single string.
   */
  [[nodiscard]] std::string getOutput() const;

  /**
   * Add a string which can be used for completion.
   */
  void addCompletionString(std::string str) { completionStrings.emplace_back(std::move(str)); }

  void setInputTextColor(Color newColor);
  void setInputTextBackgroundColor(Color newColor);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  FullColorPalette color;
  FullStyleOptions style;
  Font font = Font::Default();

 protected:
  void renderImpl() override;

 private:
  void handleSubmit();

  static int CompletionCallback(ImGuiInputTextCallbackData *data);
  std::vector<std::pair<RecordType, Record>> records;
  std::function<std::optional<Record>(std::string_view)> inputHandler = [](auto) -> std::optional<Record> {
    return {};
  };

  WrapTextToggle wrapTextToggle;
  ScrollToEndToggle scrollToEndToggle;
  CopyToClipboardButton copyToClipboardButton;
  TrashcanButton clearButton;

  constexpr static std::size_t BUFFER_SIZE = 512;
  char inputBuffer[BUFFER_SIZE] = "\0";

  std::vector<std::string> history;
  std::optional<std::size_t> historyIndex = std::nullopt;

  std::vector<std::string> completionStrings;

  std::optional<Color> inputTextColor = std::nullopt;
  std::optional<Color> inputTextBackgroundColor = std::nullopt;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_CONSOLEPANEL_H
